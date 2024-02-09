#!/bin/bash

search_files() {
  local dir="$1"
  local pattern="$2"

  local files=()  # массив для хранения директорий

  for file in "$dir"/*; do
    if [[ -d "$file" ]]; then
      # если директория, то ищем в ней
      search_files "$file" "$pattern"
    elif [[ -f "$file" ]]; then
      # если файл, то проверяем соответсвие регулярке
      if [[ "$file" =~ $pattern ]]; then
        files+=("$file")
      fi
    fi
  done

  echo "${files[@]}"
}

# проверка аргументов
if [ "$#" -ne 1 ]; then
  echo "Использование: $0 <регулярное_выражение>"
  exit 1
fi

root_directory="/home/"

regex="$1"

# ищем файлы и сохраняем в массив
found_files=($(search_files "$root_directory" "$regex"))

# проверка наличия файлов
if [ ${#found_files[@]} -eq 0 ]; then
  echo "Файлы не найдены."
  exit 0  
fi

echo -e "\nНайденные файлы:"
for ((i=0; i<${#found_files[@]}; i++)); do
  echo "$i: ${found_files[$i]}"
  echo "$i: ${found_files[$i]}" >> "save.txt"
done

read -p "Выберите номер файла для выполнения команды: " selected_file_index

# проверка на число + нахождение в пределах массива
if [[ "$selected_file_index" =~ ^[0-9]+$ && "$selected_file_index" -ge 0 && "$selected_file_index" -lt ${#found_files[@]} ]]; then
  selected_file="${found_files[$selected_file_index]}"
  echo "Выбран файл: $selected_file"

  # запрашиваем команду
  read -p "Введите команду для выполнения с файлом: " user_command

  eval "$user_command" "$selected_file"
else
  echo "Некорректный номер файла."
  exit 1
fi
