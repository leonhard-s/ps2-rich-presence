// Copyright 2022 Leonhard S.

#include "core.hpp"

namespace ps2rpc
{

    RichPresenceApp::RichPresenceApp(QObject *parent)
        : QObject(parent)
    {
        tracker_.reset(); // nullptr if no character is selected
        discord_.reset(new PresenceHandler(this));
        factory_.reset(new PresenceFactory(this));
    }

    bool RichPresenceApp::hasCharacter() const
    {
        return tracker_ != nullptr;
    }

    CharacterData RichPresenceApp::getCharacter() const
    {
        if (!hasCharacter())
            return CharacterData();
        return tracker_->getCharacter();
    }

    void RichPresenceApp::setCharacter(const CharacterData &character)
    {
        // If there is an active tracker, disconnect it
        if (tracker_)
        {
            tracker_->deleteLater();
        }
        // Create a new tracker
        tracker_.reset(new ActivityTracker(character, this));
        connect(tracker_.get(), &ActivityTracker::stateChanged,
                this, &RichPresenceApp::onGameStateChanged);
        connect(tracker_.get(), &ActivityTracker::payloadReceived,
                this, &RichPresenceApp::onEssPayloadReceived);
    }

    void RichPresenceApp::onGameStateChanged(const GameState &state)
    {
        factory_->setActivityFromGameState(state);
    }

    void RichPresenceApp::onEssPayloadReceived(const QString &event_name,
                                               const QJsonObject &payload)
    {
        // TODO: Set last ESS payload timestamp
    }

} // namespace ps2rpc
