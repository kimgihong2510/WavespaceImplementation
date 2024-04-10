#include <JuceHeader.h>

class TextComponent : public juce::Component
{
public:
    TextComponent() {};
    ~TextComponent() {};
    void paint(juce::Graphics& g) override;
    void setText(const juce::String& newText);

private:
    juce::String mText = juce::String("");
};