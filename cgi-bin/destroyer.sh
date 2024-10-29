read -r input

selected_file=$(echo "$input_data" | sed 's/+/ /g' | tr '&' '\n' | grep '^option=' | cut -d'=' -f2)

filepath=""

case "$selected_file" in
  "fileok1")
    filepath="../destroy/fileok1.txt"
    ;;
  "fileok2")
    filepath="../destroy/fileok2.txt"
    ;;
  "filenok1")
    filepath="../destroy/filenok1.txt"
    ;;
  "filenok2")
    filepath="../destroy/filenok2.txt"
    ;;
esac

curl -X DELETE filepath