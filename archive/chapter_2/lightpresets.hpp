#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lighthelper.hpp"

namespace LightPresets
{
    inline const LightEnv DESERT =
    {
        {0.75f, 0.52f, 0.3f},

        {
            {-0.2f, -1.0f, -0.3f},
            {0.3f, 0.24f, 0.14f},
            {0.7f, 0.42f, 0.26f},
            {0.5f, 0.5f, 0.5f}
        },

        {
            {
                pointLightPositions[0],
                {0.1f, 0.06f, 0.0f},
                {1.0f, 0.6f, 0.0f},
                {1.0f, 0.6f, 0.0f}
            },

            {
                pointLightPositions[1],
                {0.1f, 0.0f, 0.0f},
                {1.0f, 0.0f, 0.0f},
                {1.0f, 0.0f, 0.0f}
            },

            {
                pointLightPositions[2],
                {0.1f, 0.1f, 0.0f},
                {1.0f, 1.0f, 0.0f},
                {1.0f, 1.0f, 0.0f}
            },

            {
                pointLightPositions[3],
                {0.02f, 0.02f, 0.1f},
                {0.2f, 0.2f, 1.0f},
                {0.2f, 0.2f, 1.0f}
            }
        },

        {
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, -1.0f},

            {0.0f, 0.0f, 0.0f},
            {0.8f, 0.8f, 0.0f},
            {0.8f, 0.8f, 0.0f},

            1.0f,
            0.09f,
            0.032f,

            glm::cos(glm::radians(12.5f)),
            glm::cos(glm::radians(13.0f))
        }
    };

    inline const LightEnv FACTORY =
    {
        {0.1f, 0.1f, 0.1f},

        {
            {-0.2f, -1.0f, -0.3f},
            {0.05f, 0.05f, 0.1f},
            {0.2f, 0.2f, 0.7f},
            {0.7f, 0.7f, 0.7f}
        },

        {
            {
                pointLightPositions[0],
                {0.02f, 0.02f, 0.06f},
                {0.2f, 0.2f, 0.6f},
                {0.2f, 0.2f, 0.6f},

                1.0f,
                0.09f,
                0.032f
            },

            {
                pointLightPositions[1],
                {0.03f, 0.03f, 0.07f},
                {0.3f, 0.3f, 0.7f},
                {0.3f, 0.3f, 0.7f},

                1.0f,
                0.09f,
                0.032f
            },

            {
                pointLightPositions[2],
                {0.0f, 0.0f, 0.03f},
                {0.0f, 0.0f, 0.3f},
                {0.0f, 0.0f, 0.3f},

                1.0f,
                0.09f,
                0.032f
            },

            {
                pointLightPositions[3],
                {0.04f, 0.04f, 0.04f},
                {0.4f, 0.4f, 0.4f},
                {0.4f, 0.4f, 0.4f},

                1.0f,
                0.09f,
                0.032f
            }
        },

        {
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, -1.0f},

            {0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},

            1.0f,
            0.009f,
            0.0032f,

            glm::cos(glm::radians(10.0f)),
            glm::cos(glm::radians(12.5f))
        }
    };

    inline const LightEnv HORROR =
    {
        {0.0f, 0.0f, 0.0f},

        {
            {-0.2f, -1.0f, -0.3f},
            {0.0f, 0.0f, 0.0f},
            {0.05f, 0.05f, 0.05f},
            {0.2f, 0.2f, 0.2f}
        },

        {
            {
                pointLightPositions[0],
                {0.01f, 0.01f, 0.01f},
                {0.1f, 0.1f, 0.1f},
                {0.1f, 0.1f, 0.1f},

                1.0f,
                0.14f,
                0.07f
            },

            {
                pointLightPositions[1],
                {0.01f, 0.01f, 0.01f},
                {0.1f, 0.1f, 0.1f},
                {0.1f, 0.1f, 0.1f},

                1.0f,
                0.14f,
                0.07f
            },

            {
                pointLightPositions[2],
                {0.01f, 0.01f, 0.01f},
                {0.1f, 0.1f, 0.1f},
                {0.1f, 0.1f, 0.1f},

                1.0f,
                0.22f,
                0.20f
            },

            {
                pointLightPositions[3],
                {0.03f, 0.01f, 0.01f},
                {0.3f, 0.1f, 0.1f},
                {0.3f, 0.1f, 0.1f},

                1.0f,
                0.14f,
                0.07f
            }
        },

        {
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, -1.0f},

            {0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},

            1.0f,
            0.09f,
            0.032f,

            glm::cos(glm::radians(10.0f)),
            glm::cos(glm::radians(15.0f))
        }
    };

    inline const LightEnv BIOCHEMICAL_LAB =
    {
        {0.9f, 0.9f, 0.9f},

        {
            {-0.2f, -1.0f, -0.3f},
            {0.5f, 0.5f, 0.5f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f}
        },

        {
            {
                pointLightPositions[0],
                {0.04f, 0.07f, 0.01f},
                {0.4f, 0.7f, 0.1f},
                {0.4f, 0.7f, 0.1f},

                1.0f,
                0.07f,
                0.017f
            },

            {
                pointLightPositions[1],
                {0.04f, 0.07f, 0.01f},
                {0.4f, 0.7f, 0.1f},
                {0.4f, 0.7f, 0.1f},

                1.0f,
                0.07f,
                0.017f
            },

            {
                pointLightPositions[2],
                {0.04f, 0.07f, 0.01f},
                {0.4f, 0.7f, 0.1f},
                {0.4f, 0.7f, 0.1f},

                1.0f,
                0.07f,
                0.017f
            },

            {
                pointLightPositions[3],
                {0.04f, 0.07f, 0.01f},
                {0.4f, 0.7f, 0.1f},
                {0.4f, 0.7f, 0.1f},

                1.0f,
                0.07f,
                0.017f
            }
        },

        {
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, -1.0f},

            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},

            1.0f,
            0.07f,
            0.017f,

            glm::cos(glm::radians(7.0f)),
            glm::cos(glm::radians(10.0f))
        }
    };
}