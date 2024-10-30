read -r input

selected_file=$(echo "$input" | sed 's/+/ /g' | tr '&' '\n' | grep '^option=' | cut -d'=' -f2)

filepath=""

case "$selected_file" in
  "fileok1")
    filepath="http://127.0.0.1:8080/destroy/fileok1.txt"
    ;;
  "fileok2")
    filepath="http://127.0.0.1:8080/destroy/fileok2.txt"
    ;;
  "filenok1")
    filepath="http://127.0.0.1:8080/notdestroy/filenok1.txt"
    ;;
  "filenok2")
    filepath="http://127.0.0.1:8080/notdestroy/filenok2.txt"
    ;;
esac

# Envoie la réponse HTTP pour indiquer la fin de la requête
echo -e "HTTP/1.1 204 No Content\r\n"
echo -e "Content-Length: 0\r\n"
echo -e "Connection: close\r\n\r\n"

# Exécute la commande curl en arrière-plan, redirigeant les sorties pour éviter les messages dans le terminal
if [[ -n "$filepath" ]]; then
  curl -X DELETE "$filepath" > /dev/null 2>&1 &
fi