# TODO LIST

1. Parsear Body de entrada 
- verificar tipo de envio de info, Content-len vs chuncked

2. Parsear Header de entrada para controlar flujo contra la info del config file que ya tiene cada server fisico
- Control de errores del HTTP de entrada, para controlar el flujo del FD del cliente

3. Generar headers correctos dependiendo de respuesta
- Quizas necesite una clase adicional, aunque las bases estan

4. buscar CGI real para verificar como funciona correctamente
- posiblemente buscar de varios tipos para el bonus


5. Validar Max Size de Header.
-  Si Header error (parseer) leer hasta doble salto de linea o Max size
- Si, doble salto de linea, enviar respuesta, tratar siguientes caracteres como una solicitud nueva
- else, Max Size, se envia lo que se tenga en cola (si se tiene) y se cierra conexion.

6. Cerrar client si es POST, si NO tiene 100-Continue Y si NO es valido.
- Leemos hasta fin HTTP o Max HTTP size.
-  MaxBodyError es pequeno, unos 16000 bytes, para permitir conexiones cuando tiene error, mandaron un body, y el body es relativamente pequeño. Si es un post con error y ya han enviado el archivo y el archivo es muy grande, le cerramos. No perdemos recursos/tiempo en user is stupid. 
- Si llegamos al fin HTTP, verificamos ContentLen vs MaxBodyError
- - Si Content Len < Max Body Error, leemos y lo siguiente es una solicitud nueva
- - Si es chunked, leemos hasta que la suma sea > MaxBodyError, si es mayor entonces cerramos. 

7. Si Post errone y con 100-continue:
- Leemos hasta el HTTP end o Max HTTP 
- Si es Max HTTP cerramos conexion
- Else, damos respuesta con codigo de error, y la siguiente la tomamos como una petecion nueva, si nos mandaran el Body porque USER IS STUPID entonces parseer daria error.

8. MaxHeaderSize o MaxBody error == 413 Payload TOO large
9. BONUS COOKIES
   - En el config ponemos algo estilo if ($cookies) para identificar que el server debe de verificar el contenido de cookies en esta conexion. De esta forma podemos retornar diferentes htmls dependiendo de configuracion de lang
   - Se puede colocar una directiva de set_header Cookie + variable name? 
   - Pasar estos headers a el CGI? O lo maneja directamente el server?
   - El JS puede tambien manejar los cookies con  document.cookie

| Cookies State                 | Acciones                                                     | Status |
| ----------------------------- | ------------------------------------------------------------ | ------ |
| First Connection              | Client no envia cookies en su HTTP. Se crea una session para el cliente, se debe de guardar el SESION ID para posteriores solicitudes. Key=Value pairs | ❌      |
| Que contendria el COOKIE?     | Son Key=Value pairs. Enviamos un header Set-Cookie= Key=Value. Podemos pasar attributos con cada cookie separando con ";" See [Attributes](#Attributes) |        |
| More Connections              | Client envia TODOS los cookies que le enviamos, tanto en la ultima respuesta como en anteriores. En un campo Cookies, con valores separados por ; para cada cookie. Las cookies en si son Key=Value pairs | ❌      |
| Que hacer si tenemos cookies? | Modificar coomportamiento/respuestas dependiendo de si el cliente nos envia Cookies validas o no. | ❌      |
|                               |                                                              |        |
|                               |                                                              |        |
|                               |                                                              |        |
|                               |                                                              |        |
|                               |                                                              |        |


## Attributes

| Attribute Name                                               | Attribute Action                                             | Dev Notes | Dev State |
| ------------------------------------------------------------ | ------------------------------------------------------------ | --------- | --------- |
| Expires=sane-cookie-date ([Sane Cookie Definition](https://datatracker.ietf.org/doc/html/rfc2616#section-3.3.1)) | Sets expiration date for Cookie. Client doesnt HAVE to keep that Cookie up to that date |           | ❌         |
| Max-Age=non-zero-digit                                       | Similar to Expires. It re presents the number of seconds until it expires.<br />Si Max-age y Expires esta presente, Max Age toma prioridad. Si no se envia ninguno, user agent decide cuando dejar de guardar cookies<br /> |           | ❌         |
| Domain=domain-value ([Domain-Value Definition](https://datatracker.ietf.org/doc/html/rfc1034#section-3.5) [Domain-Value Enhancement](https://datatracker.ietf.org/doc/html/rfc1123#section-2.1)) | Le indica al cliente a que HOSTS debe de enviar este cookie. Si enviamos Domain=example.com, entonces el cliente enviara el cookie a example.com, www.example.com, www.corp.example.com, por ejemplo. <br />Sera rechazado  si es que el Domain no incluye el Origin server:<br />For example, the user agent will accept a cookie with a    Domain attribute of "example.com" or of "foo.example.com" from    foo.example.com, but the user agent will not accept a cookie with a    Domain attribute of "bar.example.com". |           | ❌         |
| Path=<any CHAR except CTLs or ";">                           | The user agent will include the cookie in an HTTP request only if the    path portion of the request-uri matches (or is a subdirectory of) the    cookie's Path attribute, where the %x2F ("/") character is    interpreted as a directory separator. |           | ❌         |
| Secure                                                       | The Secure attribute limits the scope of the cookie to "secure"    channels (where "secure" is defined by the user agent).  When a    cookie has the Secure attribute, the user agent will include the    cookie in an HTTP request only if the request is transmitted over a    secure channel (typically HTTP over Transport Layer Security (TLS)    [RFC2818]). |           | ❌         |
| HttpOnly                                                     | The HttpOnly attribute limits the scope of the cookie to HTTP    requests.  In particular, the attribute instructs the user agent to    omit the cookie when providing access to cookies via "non-HTTP" APIs    (such as a web browser API that exposes cookies to scripts).     Note that the HttpOnly attribute is independent of the Secure    attribute: a cookie can have both the HttpOnly and the Secure    attribute. | Por ejemplo, si tratamos de usar JS para leer las cookies, pero las cookies estan en HttpOnly, entonces JS **no** tendra acceso a las cookies, pero si podremos verlas en el HTTP Request!      | ❌         |



|Code | Name |  Use case |Condition | Additional Information | Dev Notes | Status |
| ---- | --|--------- | ----------------- | --| -- | -- |
|https://datatracker.ietf.org/doc/html/rfc7231#page-50|||||||
| 100  | Continue |  Client might send a big payload and wants to confirm server can handle  it | Client sends a Expect: 100-continue header action| Make sure we can serve the request| | ❌|
| 101 | Switching Protocols|Upgrade  protocol version| When we consider is best?|||❌|
|https://datatracker.ietf.org/doc/html/rfc7231#page-51|||||||
| 200| OK |Request OKAY, provides a body with the requested response| Everything was OKAY with the response and we return a representation of the status| If: GET: Target Resource, IF: POST: Representation or result, IF DELETE: Status. **Requiers a BODY. If you dont want to send BODY then set Content-Lenght: 0 or USE 204 NO CONTENT**| | ❌|
| 201| Created |Request OKAY and wnat to link response| Request OKAY|Add Location field or add URL with location. Might want to return a HTML with a link to the resource created||❌|
| 202 | Accepted |Accepted request, no response available at the moment| Requested resource takes too long and we dont want the client to wait, client will send requests in the future requiring confirmation| Handle future requests for this resource until a final response is generated||❌|
| 203| Non-Authoritative Information |Successful but modified by proxy| Request was sent to proxy and proxy modified the OG response|||❌|
| 204| No Content |Successful and NO BODY| Request processed and no body is to be sent| Similar to 200, but explicitly used for when no body is going to be sent ||❌|
| 205| Reset Content |Successful and client must reset data fields| Client must reset data fields| Probably used by CGIS since server is unlikely to know if the fields must be reset||❌|
|https://datatracker.ietf.org/doc/html/rfc7231#page-54|||||||
|300 | Multiple Choices|Redirection with multiple choices| When we want to redirect to multiple places| Provide Location field if you have a preference with the corresponding URL. **HAS** to have body with some sorf of list with URLs (usually some predefine structure) | | ❌|
| 301| Moved Permanently  | When URL has been moved permanently to another location| URL moved| **SHOULD** generate Location header, BODY contains HTML with hyperlink| | ❌|
| 302 | Found | Temporary redirection| When the resource is temporary available in another URL, maintaince for example| **SHOULD** generate Location header, BODY contains HTML with hyperlink. Diff 302/307? 302 **DOES NOT** mandate client to keep same header and payload while 307 **DOES** ||❌|
| 303| See other| Request OKAY and a representation of the request can be viewed in another URL| For some reason, we want to show the request's result in a different URL and want to **ENSURE** it's done via a GET request| **SHOULD** have location and hypertext||❌|
| 305 | Use Proxy   |   Deprecated        |      Deprecated             | Deprecated| | ✅|
| 307 | Temporary Redirect | See 302          | See 302|||❌|
|https://datatracker.ietf.org/doc/html/rfc7231#page-58|||||||
|**4xx SHOULD send a BODY with a representation of the error**      |                   ||||||
| 400| Bad Request | Will not/Can not process because of client error| Invalid Syntax on client side| This is for when we parse the HTTP and something is wrong with it. If we get to do the request to the SERVER itself, like maybe the URL being asked is not within our list, then thats a 404, for example.||✅|
|  403 | Forbidden | Bad credentials or other reason why we can't provide answer for that specific request| We understood the request, but we refuse to process it| We can send a Payload with an explanation. If we don't want to let know that this URL exists when the credentials are invalidad, we **MAY** respond with a 404 instead ||✅|
| 404 | Not Found | Couldn't find the requested URL or we want to hide it was forbidden| All okay, but we don't have that URL in our server or the credentials be lacking| If the URL has been moved permanently, we can send a 410 instead||✅|
| 405 | Method Not Allowed   | We know the method and URL, but the URL does not accept the method| All Okay but invalid method for that resource| we **MUST** send a Allow header with the valid Methods||✅|
| 406 | Not Acceptable  |  Server is **NOT** able to provide a response in the requested format| If we get a Accept field with a format we are unable to provide and have no default available| Example: Accept:application/json but we can only provide answers in Content-Type:application/xml ||✅|
|408| Request Timeout  |When we don't receive the **FULL** HTTP or BODY within the time limit | We are waiting for more input and we time out. We **MUST** send a Connection:close|IF the CGI times out or something else, we send a 500 or 504 instead|Partial Implementation, still need to parse Body on requests|✅|
|409| Conflict  |  A client wants to change a resource that's being used by a third party?| Maybe they want to edit a file (post?) that's being used by another client/third party                   ||Do we need to handle this?|✅|
|410|  Gone | The resource is GONE permanently and we want to indicate to any third-party to remove links to it| Gone and likely to not come back                   | If we don't know if it's 100% gone, a 404 is better ||✅|
|411| Length Required| Resource requires Length to be process| All Okay but we are missing Content-Length to process request||Do we need to implement this error type?|✅|
|413|Payload Too Large| Payload bigger than supported| We may check Content-len or, if chunked, if the becomes too big to handle| If the condition is temporary, we can send a Retry-After field||✅|
|414|URI Too Long| URI too long to process| When the URI is too long...|only likely to occur when a client has improperly converted a POST request to a GET request with long query information, when the client has descended into a "black hole" of redirection ||✅|
| 415| Unsupported Media Type | Format not supported on this resource| Limitation on content-type or Content-Encoding for this resource|||✅|
| 417|Expectation Failed| Unable to support Expect: field on resource|  Unable to support Expect: field on resource| Could be that they send a Expect:100-continue and that resource doesn't suppor it||✅|
| 426 | Upgrade Required| Client must upgrade it's protocol| maybe they are sending a 1.0 request and we are handling 1.1 only|**MUST** send Upgrade:  field with the protocol required||✅|
|https://datatracker.ietf.org/doc/html/rfc7231#page-62|||||||
|**5xx SHOULD send a representation of the error and if we know if it's permanent or not**        |                   | |||||
| 500 | Internal Server Error | We ran into a problem runnig the request|Whenever we have an error on our side, related to exceptions or error when executing a sys call|||✅|
| 501 | Not Implemented  |We do not support the requested Method| We do not reccognize the method request, and therfore can't serve it. We only support GET, POST and DELETE |||❌|
| 502 | Bad Gateway  | When we encounter an error from a Upstream| We parsed a response or the upstream encountered an error| Maybe the upstream crashed, maybe it timedout, maybe we parse the info and is incorrect. Can be used with CGI, but probably best not ||❌|
|503| Service Unavailable| Temporary overload or maintenance| Maybe we reached our limit of connections, or we are undergoing maintenance| **MAY** send Retry-After:. This does not mean we are OVERLOADED, just taht we refuse the connection ||❌|
|504| Gateway Timeout  |  No response before time out| Since we handle CGIs, if the CGI times out this is the response.|||❌|
| 505 |HTTP Version Not Supported  | We do not support the mayor version client is using| We are unable or unwilling to send a response using the same version.| **SHOULD** generate a representation indicating WHY and what protocols we DO support ||❌|