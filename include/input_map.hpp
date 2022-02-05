

#ifndef FU_INPUT_MAP_HPP
#define FU_INPUT_MAP_HPP

#include <SFML/System/String.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <initializer_list>
#include <unordered_map>
#include <functional>

namespace fu
{

    template <
        typename Action = std::string, 
        typename KeyEvent = sf::Keyboard::Key,
        typename Callback = std::function<void()>
    >
    class InputMap
    {
    public:
        InputMap(const std::unordered_map<Action, KeyEvent>&);

        KeyEvent&& getAction(Action&& action) const;
        void registerAction(Action&& action, KeyEvent key);
        void registerCallback(KeyEvent key, Callback&& function);
        void registerAll(Action&& action, KeyEvent key, Callback&& function);

    private:
        std::unordered_map<Action, KeyEvent> m_input;
        std::unordered_map<KeyEvent, Callback> m_callbacks;
    };

    template <typename Action, typename KeyEvent, typename Callback>
    InputMap<Action, KeyEvent, Callback>::InputMap(
            const std::unordered_map<Action, KeyEvent>& umap)
    : m_input(umap)
    { }

    template <typename Action, typename KeyEvent, typename Callback>
    KeyEvent&& InputMap<Action, KeyEvent, Callback>::getAction(Action&& action) const
    { return m_input.at(action); }

    template <typename Action, typename KeyEvent, typename Callback>
    void InputMap<Action, KeyEvent, Callback>::registerAction(Action&& action, KeyEvent key)
    {
        m_input.insert(std::forward<Action>(action), std::forward<KeyEvent>(key));
    }

    template <typename Action, typename KeyEvent, typename Callback>
    void registerCallback(KeyEvent key, Callback&& function)
    {}

    template <typename Action, typename KeyEvent, typename Callback>
    void registerAll(Action&& action, KeyEvent key, Callback&& function)
    {}
}

#endif // FU_INPUT_MAP_HPP