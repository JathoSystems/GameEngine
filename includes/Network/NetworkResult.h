class NetworkResult {
public:
    NetworkError error;
    std::string message;
    
    bool isSuccess() const { return error == NetworkError::SUCCESS; }
};