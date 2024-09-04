### SOME USEFUL COMANDS FOR THE EVALUATION
		
# Basic GET Request
-	curl -v http://localhost:8080/

# GET Request with Custom Headers:
-	curl -v -H "body-length: Value" http://localhost:8080/

# POST Form Request:
-	curl -v -X POST -d "name=paco&age=99" http://localhost:8082/form

# DELETE Request:
-	curl -v -X DELETE http://localhost:8080/resource/123

# CGI Form Request:
-	curl -v -X POST http://10.13.7.4:8082/hello.py \
     -H "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7" \
     -H "Accept-Encoding: gzip, deflate" \
     -H "Accept-Language: es-ES,es;q=0.9" \
     -H "Cache-Control: max-age=0" \
     -H "Connection: keep-alive" \
     -H "Content-Type: application/x-www-form-urlencoded" \
     -H "Cookie: theme=light; rememberMe=yes; sessionId=" \
     -H "Host: 10.13.7.4:8082" \
     -H "Origin: http://10.13.7.4:8082" \
     -H "Referer: http://10.13.7.4:8082/formCGI" \
     -H "Upgrade-Insecure-Requests: 1" \
     -H "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/125.0.0.0 Safari/537.36" \
     -d "name=JohnDoe"

## SIEGE

siege -c 50 -t 1M -b http://127.0.0.1:8080
-c 50: Simula 50 usuarios concurrentes.
-t 1M: La prueba se ejecutará durante 1 minuto.
-b: Activa el modo sin piedad (benchmark), enviando solicitudes tan rápido como sea posible.


Hacer siege configurando una petición en un archivo txt
-    siege -c 50 -t 10m -f siege_requests.txt
