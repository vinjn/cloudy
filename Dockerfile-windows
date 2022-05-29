# FROM scratch
FROM mcr.microsoft.com/windows:20H2
LABEL maintainer="vinjn.z@gmail.com"

WORKDIR C:/app
COPY vs2019/Release/demo.exe C:/app/

CMD [ "C:/app/demo.exe" ]