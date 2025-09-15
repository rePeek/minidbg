#include <cerrno>
#include <charconv>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <editline/readline.h>
#include <expected>
#include <iostream>
#include <print>
#include <span>
#include <string_view>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <system_error>
#include <unistd.h>

namespace {
enum class AttachError {
  InvalidArguments,
  InvalidPidFormat,
  InvalidPidValue,
  ForkFailed,
  PtraceAttachFailed,
  PtraceTracemeFailed,
  ExecFailed,
  WaitPidFailed,
};

std::expected<pid_t, AttachError> attach(std::span<const char *const> args) {
  if (args.size() < 2) {
    std::println(stderr, "Usage: {} <program> OR {} -p <pid>", args[0],
                 args[0]);
    return std::unexpected(AttachError::InvalidArguments);
  }

  const std::string_view flag{args[1]};
  if (flag == "-p") {
    if (args.size() < 3) {
      std::cerr << "Missing PID after -p\n";
      return std::unexpected(AttachError::InvalidArguments);
    }

    int pid = 0;
    auto [ptr, ec] =
        std::from_chars(args[2], args[2] + std::strlen(args[2]), pid);

    if (ec != std::errc{} || ptr != args[2] + std::strlen(args[2])) {
      std::println(stderr, "Invalid PID format");
      return std::unexpected(AttachError::InvalidPidFormat);
    }

    if (pid <= 0) {
      std::println(stderr, "Invalid PID format");
      return std::unexpected(AttachError::InvalidPidValue);
    }

    if (ptrace(PTRACE_ATTACH, pid, nullptr, nullptr) == -1) {
      std::println(stderr, "Could not attach: {}",
                   std::system_error(errno, std::generic_category()).what());
      return std::unexpected(AttachError::PtraceAttachFailed);
    }
    return pid;
  }

  // Fork and exec path
  const pid_t pid = fork();
  if (pid == -1) {
    std::println(stderr, "Fork failed: {}",
                 std::system_error(errno, std::generic_category()).what());
    return std::unexpected(AttachError::ForkFailed);
  }

  if (pid == 0) {
    // Child process
    if (ptrace(PTRACE_TRACEME, 0, nullptr, nullptr) == -1) {
      std::println(stderr, "Tracing failed: {}",
                   std::system_error(errno, std::generic_category()).what());
      std::exit(EXIT_FAILURE);
    }

    execlp(args[1], args[1], nullptr);
    std::println(stderr, "Exec failed: {}",
                 std::system_error(errno, std::generic_category()).what());
    std::exit(EXIT_FAILURE);
  }

  return pid;
}

void handle_command(pid_t pid, std::string_view line);
} // namespace

int main(int argc, const char **argv) {
  auto wait_for_process = [](pid_t pid) -> std::expected<void, AttachError> {
    int wait_status;
    int options = 0;
    if (waitpid(pid, &wait_status, options) < 0) {
      std::println(stderr, "WaitPid failed: {}",
                   std::system_error(errno, std::generic_category()).what());
      return std::unexpected(AttachError::WaitPidFailed);
    }
    return {};
  };

  char *line = nullptr;
  while ((line = readline("sdb> ")) != nullptr) {
    handle_command(pid, line);
    add_history(line);
    free(line);
  }

  auto result = attach(std::span(argv, argc)).and_then(wait_for_process);

  if (!result) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
