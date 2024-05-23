# TODO LIST

1. Parsear Body de entrada 
- verificar tipo de envio de info, Content-len vs chuncked

2. Parsear Header de entrada para controlar flujo contra la info del config file que ya tiene cada server fisico
- Control de errores del HTTP de entrada, para controlar el flujo del FD del cliente

3. Generar headers correctos dependiendo de respuesta
- Quizas necesite una clase adicional, aunque las bases estan

4. buscar CGI real para verificar como funciona correctamente
- posiblemente buscar de varios tipos para el bonus

