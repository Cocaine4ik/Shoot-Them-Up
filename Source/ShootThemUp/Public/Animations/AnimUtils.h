#pragma once

class AnimUtils
{
public:
    template <typename T>
    static T* FindFirstNotifyByClass(UAnimMontage* Animation)
    {
        if (!Animation)
        {
            return nullptr;
        }

        const auto NotifyEvents = Animation->Notifies;
        for (auto NotifyEvent : NotifyEvents)
        {
            const auto AnimNotify = Cast<T>(NotifyEvent.Notify);
            if (AnimNotify)
            {
                return AnimNotify;
            }
        }
        return nullptr;
    }
};