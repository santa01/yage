#ifndef RENDERABLE_H
#define	RENDERABLE_H

#include <string>

#include "Texture.h"
#include "RenderEffect.h"

class Renderable {
    public:
        Renderable();
        virtual ~Renderable();
        
        void setName(const std::string& name) {
            this->name = name;
        }
        
        const std::string& getName() const {
            return this->name;
        }
        
        Texture* getDiffuseTexture() {
            return this->diffuseTexture;
        }
        
        void setDiffuseTexture(Texture* diffuseTexture) {
            this->diffuseTexture = diffuseTexture;
        }
        
        RenderEffect* getEffect() {
            return this->effect;
        }
        
        void setEffect(RenderEffect* effect) {
            this->effect = effect;
        }
        
        bool castsShadow() const {
            return this->shadowCaster;
        }

        void setCastsShadow(bool castsShadow) {
            this->shadowCaster = castsShadow;
        }

        bool receivesShadow() const {
            return this->shadowReceiver;
        }

        void setReceivesShadow(bool receivesShadow) {
            this->shadowReceiver = receivesShadow;
        }
        
        virtual void render() = 0;
        
    protected:
        std::string name;

        Texture* diffuseTexture;
        RenderEffect* effect;
        
        bool shadowCaster;
        bool shadowReceiver;
};

#endif	/* RENDERABLE_H */
