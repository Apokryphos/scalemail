#pragma once

namespace ScaleMail
{
enum class TransitionState {
    NONE,
    FADING_OUT,
    FADED_OUT,
    FADING_IN,
};

void addTransitionTime(float elapsedSeconds);
void initializeTransition();
void renderTransition();
void setTransitionState(TransitionState state);
void setTransitionDuration(float duration);
void transitionFadeIn();
void transitionFadeOut();
}