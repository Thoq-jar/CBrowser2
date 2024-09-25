// noinspection JSUnusedGlobalSymbols

async function loadScript(url, integrity) {
    const response = await fetch(url);
    const scriptText = await response.text();
    const script = document.createElement('script');
    script.text = scriptText;

    const hash = await crypto.subtle.digest('SHA-256', new TextEncoder().encode(scriptText));
    const base64Hash = btoa(String.fromCharCode(...new Uint8Array(hash)));

    if (base64Hash === integrity) {
        document.head.appendChild(script);
        console.log("Script loaded and integrity matched!");
    } else {
        console.error("Integrity check failed!");
        console.warn(`base64: ${base64Hash} | expected: ${integrity}`)
    }
}
