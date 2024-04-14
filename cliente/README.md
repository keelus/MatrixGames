```Nota: Nuestro cliente, al utilizar mensajes con códigos y formatos personalizados, el código podría llegar a fallar si no se usa con nuestra parte de código del servidor. Actualmente funciona si recibe los códigos respetando el formato establecido al final de mensajes.c```

Para compilar:
```gcc mainCliente.c input.c mensajes.c -lws2_32 -o mainCliente.exe```
