# Use Debian as the base image
FROM debian:latest

# Update and install necessary packages
RUN apt update && apt install -y g++ make

# Set the working directory inside the container
WORKDIR /app

# Copy everything from the current directory (on your Mac) into /app (in the container)
COPY . /app

# Compile the C++ code using "make re"
RUN make re

# Expose the port your server will use (e.g., 8080)
EXPOSE 8080

# Run the server with config.yaml as the default command
CMD ["./webserv ", "config.yaml"]

# docker build -t webserv-image . 
# docker run -it -p 8080:8080 --rm webserv-image /bin/bash