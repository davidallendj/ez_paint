
#ifndef FU_MESSAGE_HPP
#define FU_MESSAGE_HPP

#include <queue>

namespace fu
{
    enum MessageFlags_
    {
        MessageFlags_CloseWindow,
    };
    
    template <typename T = MessageFlags_>
    struct Message
    {
        unsigned int id;
        T contents;
    };

    template <typename T = MessageFlags_>
    class MessageReceiver
    {
    public:
        void append(const Message<T>& message);
        void append(T&& message);
    
    protected:
        void processMessages();

    private:
        std::queue<Message<T>> m_messageQueue;
    };

    template <typename T>
    void MessageReceiver<T>::append(const Message<T>& message)
    { m_messageQueue.emplace_back(std::forward<Message<T>>(message)); }

    template <typename T>
    void MessageReceiver<T>::append(T&& message)
    {
        m_messageQueue.emplace_back(Message<T>(std::forward<T>(message)));
    }
}

#endif