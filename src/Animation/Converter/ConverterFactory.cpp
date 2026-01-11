#include "Animation/Converter/ConverterFactory.hpp"
#include "Animation/Animation.hpp"
#include "Animation/Converter/BounceConverter.hpp"
#include "Animation/Converter/EieoConverter.hpp"
#include "Animation/Converter/ElesticConverter.hpp"
#include "Animation/Converter/LerpConverter.hpp"

std::unique_ptr<IConverter> ConverterFactory::createConverter(AnimationType type) {
    switch (type) {
        case AnimationType::BOUNCE: return std::make_unique<BounceConverter>();
        case AnimationType::EIEO: return std::make_unique<EieoConverter>();
        case AnimationType::ELASTIC: return std::make_unique<ElasticConverter>();
        default: return std::make_unique<LerpConverter>();
    };
}
