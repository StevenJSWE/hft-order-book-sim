FROM golang:1.24-bookworm

# Install C compiler
RUN apt-get update && apt-get install -y gcc libc6-dev

WORKDIR /app

# Copy everything
COPY . .

# Set the CGO include path so it finds orderbook.h
ENV CGO_CFLAGS="-I/app/c_src"

RUN CGO_ENABLED=1 go build -o engine . ./c_src
CMD ["./engine"]