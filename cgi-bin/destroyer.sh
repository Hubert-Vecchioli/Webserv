read -r input

selected_file=$(echo "$input_data" | sed 's/+/ /g' | tr '&' '\n' | grep '^option=' | cut -d'=' -f2)

filepath=""

case "$selected_file" in
  "fileok1")
    filepath="http://127.0.0.1:8088/destroy/fileok1.txt"
    ;;
  "fileok2")
    filepath="http://127.0.0.1:8088/destroy/fileok2.txt"
    ;;
  "filenok1")
    filepath="http://127.0.0.1:8088/destroy/filenok1.txt"
    ;;
  "filenok2")
    filepath="http://127.0.0.1:8088/destroy/filenok2.txt"
    ;;
esac

curl -X DELETE "$filepath"