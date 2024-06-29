#include <iostream>
#include <string>
#include <curl/curl.h>
#include "json.hpp"


using json = nlohmann::json;

// Callback function for curl to write the received data into a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to get Ethereum price using CoinGecko API
std::string getEthereumPrice() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.coingecko.com/api/v3/simple/price?ids=ethereum&vs_currencies=usd");
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
    std::string priceData = getEthereumPrice();
    if (!priceData.empty()) {
        auto jsonData = json::parse(priceData);
        double price = jsonData["ethereum"]["usd"];
        std::cout << "Current Ethereum price in USD: " << price << std::endl;
    } else {
        std::cerr << "Failed to retrieve Ethereum price." << std::endl;
    }

    return 0;
}

