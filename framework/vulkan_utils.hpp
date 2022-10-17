#pragma once

#include "logging.hpp"

#include <vulkan/vulkan.h>

static void check_vk_result(VkResult err)
{
    if (err == 0)
    {
        return;
    }

    logging::error("Vulkan Error: VkResult = %d", err);
    if (err < 0)
    {
        abort();
    }
}