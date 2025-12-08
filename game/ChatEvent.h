#pragma once
#include "Events/IEvent.h"
#include "Network/Packet/Buffer.h"
#include <string>

class ChatEvent : public IEvent {
private:
    std::string message;
    int32_t senderId;

public:
    ChatEvent() : senderId(0) {
        name = "ChatEvent";
    }

    ChatEvent(const std::string& msg, int32_t id = 0) 
        : message(msg), senderId(id) {
        name = "ChatEvent";
    }

    std::string getName() const override { 
        return name; 
    }

    Package serialize() const override {
        Buffer buffer;
        buffer.writeInt(senderId);
        buffer.writeString(message);
        return buffer.getData();
    }

    Data deserialize(const Package& package) override {
        Buffer buffer;
        buffer.setData(package);
        
        size_t offset = 0;
        senderId = buffer.readInt(offset);
        message = buffer.readString(offset);
        
        return package;
    }

    void apply(std::shared_ptr<GameObject> gameObject) override {
        // Voor chat hoeven we niks te doen met GameObjects
        // De UI/console handled dit
    }

    // Getters
    std::string getMessage() const { return message; }
    int32_t getSenderId() const { return senderId; }
};