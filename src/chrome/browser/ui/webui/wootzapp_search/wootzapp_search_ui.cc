#include "chrome/browser/ui/webui/wootzapp_search/wootzapp_search_ui.h"

#include "base/android/jni_string.h"
#include "chrome/browser/ui/webui/webui_util.h"
#include "chrome/common/webui_url_constants.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/web_contents.h"
#include "chrome/grit/wootzapp_search_resources.h"
#include "chrome/grit/wootzapp_search_resources_map.h"
#include "content/public/browser/web_ui.h"
#include "content/public/browser/web_ui_data_source.h"
#include "chrome/browser/history/history_service_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "components/history/core/browser/history_service.h"
#include "chrome/android/chrome_jni_headers/OpenExtensionsById_jni.h"

namespace {
void CreateAndAddWootzappSearchHTMLSource(Profile* profile) {
    content::WebUIDataSource* source = content::WebUIDataSource::CreateAndAdd(
        profile,
        chrome::kChromeUIWootzappSearchHost);

    // Add required resources.
    webui::SetupWebUIDataSource(
        source,
        base::make_span(kWootzappSearchResources, kWootzappSearchResourcesSize),
        IDR_WOOTZAPP_SEARCH_WOOTZAPP_SEARCH_HTML);


    source->AddString("message", "Wootzapp Search!");
}
}  // namespace

WootzappSearchUI::WootzappSearchUI(content::WebUI* web_ui)
    : content::WebUIController(web_ui) {
    web_ui->AddMessageHandler(std::make_unique<WootzappMessageHandler>(web_ui));

    CreateAndAddWootzappSearchHTMLSource(Profile::FromWebUI(web_ui));
}

WootzappSearchUI::~WootzappSearchUI() {

}

WootzappMessageHandler::WootzappMessageHandler(content::WebUI* web_ui)
    : web_ui_(web_ui), weak_factory_(this) {
    Profile* profile = Profile::FromWebUI(web_ui_);
    history::HistoryService* history_service =
        HistoryServiceFactory::GetForProfile(profile, ServiceAccessType::EXPLICIT_ACCESS);
    if (history_service) {
        history_service->AddObserver(this);
    }
}

WootzappMessageHandler::~WootzappMessageHandler() {
    Profile* profile = Profile::FromWebUI(web_ui_);
    history::HistoryService* history_service =
        HistoryServiceFactory::GetForProfile(profile, ServiceAccessType::EXPLICIT_ACCESS);
    if (history_service) {
        history_service->RemoveObserver(this);
    }
}

void WootzappMessageHandler::RegisterMessages() {
  web_ui()->RegisterMessageCallback(
      "openGoogleInAndroidWebView",
      base::BindRepeating(&WootzappMessageHandler::HandleOpenGoogleInAndroidWebView,
                          base::Unretained(this)));
}

void WootzappMessageHandler::HandleOpenGoogleInAndroidWebView(const base::Value::List& args) {
    const std::string& url = args[0].GetString();
    
    OpenUrlInAndroidWebView(url);
}

void WootzappMessageHandler::OpenUrlInAndroidWebView(const std::string& url) {
    JNIEnv* env = base::android::AttachCurrentThread();
    base::android::ScopedJavaLocalRef<jstring> jurl = base::android::ConvertUTF8ToJavaString(env, url);
    Java_OpenExtensionsById_openExtensionWebView(env, jurl);
}