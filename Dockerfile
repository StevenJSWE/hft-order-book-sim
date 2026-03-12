FROM golang:1.24-bookworm

# Install C compiler
RUN apt-get update && apt-get install -y gcc libc6-dev

WORKDIR /app

# Copy everything
COPY . .

# Build only the root. main.go handles pulling in the c_src logic.
RUN CGO_ENABLED=1 go build -o engine .

CMD ["./engine"]