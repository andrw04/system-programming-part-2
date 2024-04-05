#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include "file_utils.c"


#define DELAY 1
#define MAX_SIGNALS 10
#define CONFIG_FILE "signals.txt"
#define LOG_FILE "signal_log.txt"
#define PID_FILE "daemon_pid.txt"


void read_config_file();
void log_signal(int sig_num);
void sighup_handler(int sig_num);
void sigterm_handler(int sig_num);
void setup_signal_handlers();
void unload_daemon();