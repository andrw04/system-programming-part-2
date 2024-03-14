#!/bin/bash

add_prefix_to_file() {
    local filename=$1
    local prefix=$2

    # Проверяем, существует ли файл
    if [ -e "$filename" ]; then
        # Формируем новое имя с префиксом
        new_filename="${prefix}.${filename}"

        # Переименовываем файл
        mv "$filename" "$new_filename"

        echo "The file $filename has been successfully renamed to $new_filename"
    else
        echo "File $filename is not found"
    fi
}

remove_prefix_from_file() {
    local filename=$1

    # Извлекаем префикс из имени файла
    prefix="${filename%%.*}"

    # Проверяем, является ли префикс числом
    if [[ "$prefix" =~ ^[0-9]+$ ]]; then
        # Проверяем, существует ли файл
        if [ -e "$filename" ]; then
            # Проверяем, что в имени файла есть как минимум две точки
            if [[ "$filename" == *.*.* ]]; then
                # Формируем новое имя без префикса
                new_filename="${filename#*.}"

                # Переименовываем файл
                mv "$filename" "$new_filename"

                echo "The prefix $prefix has been successfully removed from $filename. New name: $new_filename"
            else
                echo "Error: The file $filename does not contain a prefix"
            fi
        else
            echo "Find $filename не is not found"
        fi
    fi
}

# Функция для считывания и сортировки файлов
get_sorted_files() {
    local sort_option=$1
    local sort_order=$2
    local files=()

    case $sort_option in
        "date")
            # Сортировка по дате
            files=($(ls -lt --time=atime | awk '{print $9}'))
            ;;
        "size")
            # Сортировка по размеру
            files=($(ls -lS | awk '{print $9}'))
            ;;
        "name")
            # Сортировка по имени
            files=($(ls))
            ;;
        *)
            echo "Incorrect sort option. Use 'date', 'size' or 'name'"
            exit 1
            ;;
    esac

    # Проверяем порядок сортировки и инвертируем массив, если нужно
    if [ "$sort_order" = "desc" ]; then
        reversed_files=()
        for ((i=${#files[@]}-1; i>=0; i--)); do
            reversed_files+=("${files[$i]}")
        done
        files=("${reversed_files[@]}")
    fi

    # Возвращаем отсортированный список файлов
    echo "${files[@]}"
}

add_prefix_to_files() {
    local files=("${@}")
    local file_count=${#files[@]}

    # Определяем длину префикса исходя из количества файлов
    local prefix_length=$(( ${#file_count} + 1 ))

    local current_prefix=0
    local formatted_prefix=""

    for file in "${files[@]}"; do
        if [ "$file" != "${BASH_SOURCE[0]}" ]; then
            # Форматируем префикс с дополнением слева нулями
            formatted_prefix=$(printf "%0${prefix_length}d" "$current_prefix")

            # Добавляем префикс к файлу с использованием вспомогательной функции
            add_prefix_to_file "$file" "$formatted_prefix"

            # Увеличиваем префикс для следующего файла
            ((current_prefix++))
        fi
    done
}

remove_prefix_from_files() {
    local files=("${@}")

    for file in "${files[@]}"; do
        # Удаляем префикс из файла с использованием вспомогательной функции
        remove_prefix_from_file "$file"
    done
}

# Обработка аргументов скрипта
if [ "$#" -eq 0 ]; then
    echo "Error: Use '$0 remove' or '$0 add <sort_option> <sort_order>'"
    exit 1
fi

case "$1" in
    "remove")
        # Проверка на наличие дополнительных аргументов
        if [ "$#" -ne 1 ]; then
            echo "Error: Use '$0 remove'"
            exit 1
        fi
        remove_prefix_from_files $(ls)
        ;;
    "add")
        # Проверка на наличие дополнительных аргументов
        if [ "$#" -ne 3 ]; then
            echo "Error: Use 'add <sort_option> <sort_order>'"
            exit 1
        fi
        sort_option="$2"
        sort_order="$3"
        files=($(get_sorted_files "$sort_option" "$sort_order"))
        add_prefix_to_files "${files[@]}"
        ;;
    *)
        echo "Error: Use '$0 remove' or '$0 add <sort_option> <sort_order>'"
        exit 1
        ;;
esac
