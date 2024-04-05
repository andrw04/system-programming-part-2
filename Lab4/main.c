#include "main.h"


int signals[MAX_SIGNALS];
int num_signals = 0;


void read_config_file() {
    FILE* file = fopen(CONFIG_FILE, "r");
    if (!file) {
        exit(1);
    }

    num_signals = 0;
    char buffer[256];
    while(fgets(buffer, sizeof(buffer), file) != NULL && num_signals < MAX_SIGNALS) {
        signals[num_signals++] = atoi(buffer);
    }

    fclose(file);
}


void log_signal(int sig_num) {
    FILE* file = fopen(LOG_FILE, "a");
    if (!file) {
        exit(1);
    }

    time_t current_time = time(NULL);
    char* c_time_string = ctime(&current_time);
    c_time_string[strlen(c_time_string) - 1] = '\0';

    fprintf(file, "[%s] Signal %d received\n", c_time_string, sig_num);

    fclose(file);
}


void sighup_handler(int sig_num) {
    setup_signal_handlers();
}


void sigterm_handler(int sig_num) {
    pid_t daemon_pid = getpid();

    FILE* file = fopen(LOG_FILE, "a");

    if (!file) {
        exit(1);
    }

    fprintf(file, "Stopped Process ID: %d", daemon_pid);

    fclose(file);

    exit(0);
}


void setup_signal_handlers() {
    for (int i = 1; i < NSIG; i++) {
        signal(i, SIG_IGN);
    }

    // Установка обработчика сигнала SIGHUP
    signal(SIGHUP, sighup_handler);

    // Установка обработчика сигнала SIGTERM
    signal(SIGTERM, sigterm_handler);

    read_config_file();

    FILE* file = fopen(LOG_FILE, "a");

    if (!file) {
        exit(1);
    }

    for (int i = 0; i < num_signals; i++) {
        signal(signals[i], log_signal);
        fprintf(file, "Process %d tracks signal %d\n", getpid(), signals[i]);
    }

    fclose(file);
}


void unload_daemon() {
    pid_t daemon_pid = extract_pid(LOG_FILE);

    kill(daemon_pid, SIGTERM);

    exit(0);
}


int main(int argc, char* argv[]) {
    // Проверка опций командной строки
    if (argc > 1 && (strcmp(argv[1], "-q") == 0 || strcmp(argv[1], "--quit") == 0)) {
        unload_daemon();
    }

    pid_t pid, sid;

    pid = fork(); // создание дочернего процесса

    if (pid < 0) { // ошибка при создании процесса
        exit(1);
    }

    if (pid > 0) { // завершение если родительский процесс
        exit(0);
    }

    sid = setsid(); // создание новой сессии
    if (sid < 0) {  // ошибка при создании сессии
        exit(1);
    }

    // Установка маски создания файлов
    umask(0);

    // Закрытие стандартных потоков ввода/вывода/ошибок
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    write_start_message(LOG_FILE, sid);

    setup_signal_handlers();

    while (1) {
        sleep(DELAY);
    }
}