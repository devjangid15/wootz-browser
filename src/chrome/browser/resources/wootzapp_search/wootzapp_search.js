document.addEventListener('DOMContentLoaded', function() {
    // DOM elements
    const aiTab = document.getElementById('ai-tab');
    const googleTab = document.getElementById('google-tab');
    const aiResults = document.getElementById('aiResults');
    const googleResults = document.getElementById('googleResults');
    const searchInput = document.getElementById('searchInput');
    const searchButton = document.getElementById('searchButton');

    document.getElementById('openInAndroidBtn').addEventListener('click', function() {
        const query = document.getElementById('searchInput').value;
        const url = `https://www.google.com/search?q=${encodeURIComponent(query)}`;
        chrome.send('openGoogleInAndroidWebView', [url]);
        console.log('Open in Android WebView clicked');
    });

    let currentTab = 'ai';
    let currentQuery = '';
    let loading = false;

    // --- Gemini API ---
    const GEMINI_API_KEY = "AIzaSyCoNFODrVovsQEFa4nseHbv0d56eMqhtDU"; // <-- PUT YOUR KEY HERE
    async function fetchGemini(query) {
        try {
            const apiUrl = `https://generativelanguage.googleapis.com/v1/models/gemini-1.5-flash:generateContent?key=${GEMINI_API_KEY}`;
            const body = {
                contents: [{
                    parts: [{
                        text: `Provide a comprehensive and helpful answer for this search query: "${query}". Include key information, context, and relevant details in 2-3 paragraphs. Format your response clearly with important points.`
                    }]
                }],
                generationConfig: {
                    temperature: 0.7,
                    topK: 40,
                    topP: 0.95,
                    maxOutputTokens: 800,
                },
                safetySettings: [
                    { category: "HARM_CATEGORY_HARASSMENT", threshold: "BLOCK_MEDIUM_AND_ABOVE" },
                    { category: "HARM_CATEGORY_HATE_SPEECH", threshold: "BLOCK_MEDIUM_AND_ABOVE" },
                    { category: "HARM_CATEGORY_SEXUALLY_EXPLICIT", threshold: "BLOCK_MEDIUM_AND_ABOVE" },
                    { category: "HARM_CATEGORY_DANGEROUS_CONTENT", threshold: "BLOCK_MEDIUM_AND_ABOVE" }
                ]
            };
            const res = await fetch(apiUrl, {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify(body)
            });
            if (!res.ok) {
                const errorData = await res.json().catch(() => ({}));
                throw new Error(`Gemini API Error: ${res.status} - ${errorData.error?.message || 'Unknown error'}`);
            }
            const data = await res.json();
            return data?.candidates?.[0]?.content?.parts?.[0]?.text || "No response from Gemini API";
        } catch (error) {
            console.error("Gemini API Error:", error);
            return `Error: ${error.message}`;
        }
    }

    // --- Tab switching ---
    aiTab.addEventListener('click', function() {
        setTab('ai');
    });
    googleTab.addEventListener('click', function() {
        setTab('google');
    });

    function setTab(tab) {
        currentTab = tab;
        if (tab === 'ai') {
            aiTab.classList.add('active');
            googleTab.classList.remove('active');
            aiResults.classList.add('active');
            googleResults.classList.remove('active');
        } else {
            googleTab.classList.add('active');
            aiTab.classList.remove('active');
            googleResults.classList.add('active');
            aiResults.classList.remove('active');
        }
        // Show results for current query if any
        if (currentQuery) {
            if (tab === 'ai') showAIResults(currentQuery);
            else showGoogleResults(currentQuery);
        }
    }

    // --- Search handling ---
    searchButton.addEventListener('click', handleSearch);
    searchInput.addEventListener('keypress', function(e) {
        if (e.key === 'Enter') handleSearch();
    });

    function handleSearch() {
        const query = searchInput.value.trim();
        if (!query) return;
        currentQuery = query;
        if (currentTab === 'ai') showAIResults(query);
        else showGoogleResults(query);
    }

    // --- AI Results ---
    async function showAIResults(query) {
        aiResults.innerHTML = `<div class="loading"></div>`;
        loading = true;
        const result = await fetchGemini(query);
        loading = false;
        aiResults.innerHTML = `
            <div class="ai-result">
                <h3>AI Response for: "${query}"</h3>
                <p>${result}</p>
            </div>
        `;
    }

    // --- Google Results (iframe + widget fallback) ---
    function showGoogleResults(query) {
        const googleSearchUrl = `https://www.google.com/search?q=${encodeURIComponent(query)}`;
        googleResults.innerHTML = `
            <div class="google-iframe-container">
                <div class="iframe-header">
                    <h3>🔍 Google Search Results for: "${query}" (Iframe)</h3>
                    <div class="iframe-actions">
                        <a href="${googleSearchUrl}" target="_blank" class="external-link">
                            🔗 Open in New Tab
                        </a>
                        <button onclick="window.refreshGoogleResults && window.refreshGoogleResults()" class="refresh-button">
                            🔄 Refresh
                        </button>
                    </div>
                </div>
                <div class="iframe-wrapper">
                    <iframe 
                        src="${googleSearchUrl}" 
                        class="google-iframe"
                        title="Google Search Results"
                        sandbox="allow-scripts allow-same-origin allow-forms allow-popups allow-popups-to-escape-sandbox">
                    </iframe>
                </div>
                <div class="iframe-blocked">
                    <h3>Iframe Blocked?</h3>
                    <p>If you don't see Google results above, your browser is blocking the iframe for security reasons.</p>
                    <p>Try the widget below or <a href="${googleSearchUrl}" target="_blank" class="fallback-link">open Google in a new tab</a>.</p>
                </div>
            </div>
            <div class="google-widget-container">
                <div class="widget-header">
                    <h3>🔍 Google Search Results (Widget Fallback)</h3>
                    <p>Searching for: "${query}"</p>
                </div>
                <div class="google-search-widget">
                    <script async src="https://cse.google.com/cse.js?cx=017576662512468239146:omuauf_lfve"></script>
                    <div class="gcse-search" data-queryParameterName="q" data-defaultQuery="${query}"></div>
                </div>
            </div>
        `;
        // Re-inject the widget script (needed for dynamic content)
        setTimeout(() => {
            const script = document.createElement('script');
            script.src = "https://cse.google.com/cse.js?cx=017576662512468239146:omuauf_lfve";
            script.async = true;
            googleResults.querySelector('.google-search-widget').prepend(script);
        }, 0);
    }

    // For refresh button in iframe
    window.refreshGoogleResults = function() {
        if (currentQuery) showGoogleResults(currentQuery);
    };

    // --- Initial state ---
    setTab('ai');
}); 