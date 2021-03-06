#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

static const char *gBasicShaderProgram = R"BSV(
{
  "ShaderProgram": {
    "DescriptorSetLayouts": [
      {
        "UniformDescriptors": [
          {
            "Name": "camera",
            "Type": "buffer",
            "Member": [
              {
                "Name": "proj",
                "Type": "mat4"
              },
              {
                "Name": "view",
                "Type": "mat4"
              }
            ]
          }
        ]
      },
      {
        "UniformDescriptors": [
          {
            "Name": "geometry",
            "Type": "buffer",
            "Member": [
              {
                "Name": "world",
                "Type": "mat4"
              },
              {
                "Name": "normal",
                "Type": "mat4"
              }
            ]
          }
        ]
      },
      {
        "UniformDescriptors": [
          {
            "Name": "light",
            "Type": "buffer",
            "Member": [
              {
                "Name": "ambient",
                "Type": "color"
              },
              {
                "Name": "diffuse",
                "Type": "color"
              },
              {
                "Name": "specular",
                "Type": "color"
              },
              {
                "Name": "position",
                "Type": "vec4"
              },
              {
                "Name": "direction",
                "Type": "vec4"
              },
              {
                "Name": "spotExponent",
                "Type": "float"
              },
              {
                "Name": "spotCosCutoff",
                "Type": "float"
              },
              {
                "Name": "constantAttenuation",
                "Type": "float"
              },
              {
                "Name": "linearAttenuation",
                "Type": "float"
              },
              {
                "Name": "quadraticAttenuation",
                "Type": "float"
              },
              {
                "Name": "kind",
                "Type": "int"
              }
            ]
          }
        ]
      },
      {
        "UniformDescriptors": [
          {
            "Name": "material",
            "Type": "buffer",
            "Member": [
              {
                "Name": "ambient",
                "Type": "color"
              },
              {
                "Name": "diffuse",
                "Type": "color"
              },
              {
                "Name": "specular",
                "Type": "color"
              },
              {
                "Name": "emission",
                "Type": "color"
              },
              {
                "Name": "shininess",
                "Type": "float"
              }
            ]
          },
          {
            "Name": "textures",
            "Type": "images",
            "number": 11
          }
        ]
      }
    ],
    "Pipelines": [
      {
        "PipelineName": "ForwardBasicShading_P0",
        "VertShader": "/shader/ForwardBasicShading_P0.vert.spv",
        "TessCtrlShader": "",
        "TessEstShader": "",
        "GeoShader": "",
        "FragShader": "/shader/ForwardBasicShading_P0.frag.spv",
        "GraphicsParam": {}
      },
      {
        "PipelineName": "ForwardBasicShading_P1",
        "VertShader": "/shader/ForwardBasicShading_P1.vert.spv",
        "TessCtrlShader": "",
        "TessEstShader": "",
        "GeoShader": "",
        "FragShader": "/shader/ForwardBasicShading_P1.frag.spv",
        "GraphicsParam": {}
      }
    ],
    "RenderPassInfos": [
      {
        "TargetPass": "ForwardPass",
        "SubpassInfos": [
          {
            "Name": "FirstLight",
            "UsedDescriptorSetIDs": [
              0,
              1,
              2,
              3
            ],
            "PipelineID": 0
          },
          {
            "Name": "SecondaryLight",
            "UsedDescriptorSetIDs": [
              0,
              1,
              2,
              3
            ],
            "PipelineID": 1
          },
          {
            "Name": "Transparent",
            "UsedDescriptorSetIDs": [
              0,
              1,
              2,
              3
            ],
            "PipelineID": 1
          }
        ]
      }
    ]
  }
}

)BSV";

______________SD_END_GRAPHICS_NAMESPACE______________