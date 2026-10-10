# Badge System (Geode)

Mod de Geometry Dash que muestra insignias (Owner, Admin, Moderator, Helper, VIP, Contributor)
en el perfil del jugador, al lado del nombre, como la insignia de moderador de RobTop.
Tocando la insignia se abre un cuadro con su nombre y descripción.

## Cómo funciona
El mod descarga un archivo `badges.json` desde una URL que controlas tú (por ejemplo, un archivo
en tu repositorio de GitHub). Ese archivo dice qué insignias existen y quién tiene cuáles.
Como solo tú puedes editar ese archivo, **solo tú das las insignias**.

Si no hay internet, el mod usa la última copia guardada; si nunca descargó nada, usa el
`resources/badges.json` incluido dentro del mod.

## Preparar el proyecto
1. En `mod.json` cambia `id` (formato `tunombre.badgesystem`) y `developer`.
2. Instala el Geode SDK y la CLI: https://docs.geode-sdk.org/getting-started/
3. Compila: `geode build` (genera un `.geode` en `build/`). Copia ese archivo a la carpeta `geode/mods` de tu juego.
   Alternativa sin instalar nada: sube el proyecto a GitHub; la acción de `.github/workflows/build.yml` compila el `.geode`.

## Dar insignias (tú, como owner)
1. Sube `badges.json` a un repositorio de GitHub.
2. Abre el archivo en GitHub, pulsa "Raw" y copia esa URL.
3. Pégala como valor por defecto de `badges-url` en `mod.json` (reemplazando `TU_USUARIO/TU_REPO`)
   y recompila. También se puede cambiar en los ajustes del mod dentro del juego.
4. Para dar una insignia, añade al arreglo `users` el **Account ID** del jugador:

```json
{ "accountID": 123456, "badges": ["owner"] },
{ "accountID": 654321, "badges": ["moderator", "helper"] }
```

El Account ID no es el User ID: es el que aparece en la URL/datos de la cuenta (por ejemplo, en
GDBrowser aparece como "Account ID"). Las insignias aparecen en el mismo orden en que las escribas.
GitHub tarda unos minutos en refrescar el archivo raw; los jugadores lo reciben al abrir el juego.

## Crear insignias nuevas
1. Añade tu imagen PNG a `resources/` (idealmente `nombre.png`, `nombre-hd.png`, `nombre-uhd.png` de 32, 64 y 128 px).
2. Añade una entrada en `badges` de `badges.json` con `id`, `name`, `description` y `sprite`.
3. Recompila el mod y súbelo/distribúyelo: las imágenes viajan dentro del mod; los textos y los dueños, en el JSON online.

## Archivos
- `src/main.cpp`: arranque del mod.
- `src/BadgeManager.*`: carga y actualización de `badges.json`.
- `src/ProfileHook.cpp`: dibuja las insignias en el perfil.
- `resources/`: iconos de las insignias y el `badges.json` de respaldo.
- `badges.json`: plantilla para subir online.
