#include "Glow.h"
#include <iostream>

#include "../../sdk/Level.h"
#include "../../sdk/LocalPlayer.h"
#include "../../sdk/Entity.h"
#include "../../sdk/EntityList.h"

Glow::Glow() : Module(true, "Glow") {}

void Glow::OnEnable() {
    std::cout << "Enabling Glow" << std::endl;
}

void Glow::OnDisable() {
    std::cout << "Disabling Glow" << std::endl;
}

#define LOG(x) std::cout << x << std::endl;

void Glow::Update() {
    

    if (!Level::isPlayable())
        return;

    if (!LocalPlayer::IsAlive())
        return;

    if (LocalPlayer::IsKnocked())
        return;

    if(!(LocalPlayer::GetTeamNumber() > 0 && LocalPlayer::GetTeamNumber() < 32))
        return;

    for(int i = 0; i < EntityList::GetSize(); i++)
    {
        Entity *ent = EntityList::GetEntity(i);

        if (!ent->isValid())
            continue;
            
        if (ent->GetTeamNumber() == LocalPlayer::GetTeamNumber())
            continue;

        if (!(ent->getHealth() >= 0 || ent->getHealth() <= 100) || !(ent->GetTeamNumber() > 0 && ent->GetTeamNumber() < 32))
            continue;

        if(ModuleManager::healthBased)
        {
            Vector3 color = {5.f, 2.f, 7.f};

            if (ent->isKnocked() != 0 && ent->getHealth() != 0) {
                // Enemy, Downed/Dead(?), Yellow
                color = { 3.f, 3.f, 0.f };
            }
            else {
                // Enemy, Alive   
                int shield = ent->getShield();
                int health = ent->getHealth();
                if (shield > 100) { //Heirloom armor - Red
                    color = { 5.f, 0.f, 0.f };
                }
                else if (shield > 75) { //Purple armor - Purple
                    color = { 1.84f, 0.46f, 2.07f };
                }
                else if (shield > 50) { //Blue armor - Light blue
                    color = { 0.39f, 1.77f, 2.85f };
                }
                else if (shield > 0) { //White armor - White
                    color = { 3.f, 3.f, 3.f };
                }
                else if (health > 50) { //Above 50% HP - Orange
                    color = { 3.f, 1.8f, 0.f };
                }
                else { //Below 50% HP - Light Red
                    color = { 2.88f, 0.78f, 0.63f };
                }
            }

            ent->setGlow(1, 2, color, 99999);

        }
        else{
            if(ent->isVisible())
                ent->setGlow(1, 2, ModuleManager::visible, 99999);

            if(!ent->isVisible())
                ent->setGlow(1, 2, ModuleManager::hidden, 99999);
        }
        
    }
}

void Glow::hand_model_glow()
    {
        if (!Level::isPlayable())
            return;

        Vector3 color {5.f , 0.f, 0.f};

        int ViewModelHandle = LocalPlayer::ViewModelHandle();

        Entity* ViewModel = EntityList::GetEntity(ViewModelHandle* 0x20);

        color = Rainbow(0.007f);
        
        

        ViewModel->setHandGlow(1,2,{126, 101, 46, 0}, color); // {109, 112, 114}
    }

    

Vector3 Glow::Rainbow(float delay)
{
    static uint32_t cnt = 0;
    float freq = delay;

    if (++cnt >= (uint32_t)-1)
    {
        cnt = 0;
    }
    if (delay != oldDelay)
    {
        cnt = 0;
        oldDelay = delay;
    }
    Vector3 cur = {std::sin(freq * cnt + 0) * 1.3f, std::sin(freq * cnt + 2) * 1.6f, std::sin(freq * cnt + 4) * 1.9f};

    return cur;
}