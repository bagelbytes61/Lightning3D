#ifndef LG_PIPELINE_STATE_H_
#define LG_PIPELINE_STATE_H_

#pragma once

#include "LGObject.h"

namespace Lightning3D {
    struct PipelineStateDescription {


    };

    class PipelineState : public Object {
    public:
        virtual ~PipelineState() override = 0;

    protected:
    };
}

#endif