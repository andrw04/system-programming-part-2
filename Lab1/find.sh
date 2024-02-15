#!/bin/bash

# Проверяем, передан ли аргумент
if [ $# -eq 0 ]; then
    echo "Usage: $0 <filename1> [<filename2> <filename3> ...]"
    exit 1
fi

# Функция для поиска файла в директории
search_file() {
    local filename="$1"
    local dir="$2"

    # Переменная для хранения суммы байтов файла
    local total_bytes=0

    # Перебираем все файлы и директории в текущей директории
    for entry in "$dir"/*; do
        if [ -f "$entry" ]; then
            # Если это файл и его имя совпадает с искомым, выводим его содержимое с заголовком и считаем сумму байтов
            if [ "$(basename "$entry")" = "$filename" ]; then
                echo "Found: $entry"
                print_file_content_with_header "$entry"
                # Считаем сумму байтов файла и добавляем к общей сумме
                local bytes=$(wc -c < "$entry")
                total_bytes=$((total_bytes + bytes))
            fi
        elif [ -d "$entry" ]; then
            # Если это директория, рекурсивно вызываем эту же функцию для неё
            search_file "$filename" "$entry"
        fi
    done

    # Если файлы не найдены, выводим сообщение об этом
    if [ $total_bytes -ne 0 ]; then
        # Выводим сумму байтов файла
        echo "Total bytes for $filename: $total_bytes"
    fi
}

# Функция для вывода содержимого файла с заданным заголовком
print_file_content_with_header() {
    local file="$1"
    local header="#!/bin/bash"  # Укажите ваш заголовок здесь

    # Проверяем, содержит ли файл заданный заголовок
    if head -n1 "$file" | grep -q "^$header$"; then
        echo "=== Content of $file ==="
        # Выводим содержимое файла с пронумерованными строками
        cat -n "$file"
        echo "========================="
    fi
}

# Функция для поиска нескольких файлов из списка
search_files() {
    local filenames=("$@")
    local dir="$1"
    shift

    # Перебираем все имена файлов из списка
    for filename in "${filenames[@]}"; do
        search_file "$filename" "$dir"
    done
}

# Вызываем функцию для начала поиска с текущей директории
search_files "$(pwd)" "$@"
