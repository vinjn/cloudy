FROM nvidia/opengl:1.2-glvnd-runtime-ubuntu20.04

ADD build/demo /app/
WORKDIR /app/

# CMD ["demo"]