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
        
        virtual void render() = 0;
        
    protected:
        std::string name;

        Texture* diffuseTexture;
        RenderEffect* effect;
};

#endif	/* RENDERABLE_H */
