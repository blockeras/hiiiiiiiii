#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string getBitcoinPrice() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
            return "";
        }
    }

    return readBuffer;
}

int main() {
    std::string priceData = getBitcoinPrice();
    if (!priceData.empty()) {
        auto jsonData = json::parse(priceData);
        double price = jsonData["bitcoin"]["usd"];
        std::cout << "Current Bitcoin price in USD: " << price << std::endl;
    } else {
        std::cerr << "Failed to retrieve Bitcoin price." << std::endl;
    }

    return 0;
}
















