FROM alpine

ADD build/demo /app/
WORKDIR /app/

# CMD ["demo"]