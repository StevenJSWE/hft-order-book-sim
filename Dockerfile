FROM golang:1.22-bookworm

# Install C compiler
RUN apt-get update && apt-get install -y gcc libc6-dev

WORKDIR /app

# Copy everything
COPY . .

# Build the app (CGO_ENABLED=1 is the magic switch)
RUN CGO_ENABLED=1 go build -o engine main.go

CMD ["./engine"]