#ifndef CHROME_BROWSER_UI_WEBUI_WOOTZAPP_SEARCH_WOOTZAPP_SEARCH_UI_H_
#define CHROME_BROWSER_UI_WEBUI_WOOTZAPP_SEARCH_WOOTZAPP_SEARCH_UI_H_

#include "content/public/browser/web_ui_controller.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "components/history/core/browser/history_service_observer.h"

// The WebUI for chrome://wootzapp-search
class WootzappSearchUI : public content::WebUIController {
 public:
  explicit WootzappSearchUI(content::WebUI* web_ui);
  ~WootzappSearchUI() override;
};

class WootzappMessageHandler : public content::WebUIMessageHandler,
                                public history::HistoryServiceObserver {
 public:
  explicit WootzappMessageHandler(content::WebUI* web_ui);
  ~WootzappMessageHandler() override;
  
  void OpenUrlInAndroidWebView(const std::string& url);
  void HandleOpenGoogleInAndroidWebView(const base::Value::List& args);

  void RegisterMessages() override;

  raw_ptr<content::WebUI> web_ui_;
  base::WeakPtrFactory<WootzappMessageHandler> weak_factory_{this};
};

#endif // CHROME_BROWSER_UI_WEBUI_WOOTZAPP_SEARCH_WOOTZAPP_SEARCH_UI_H_