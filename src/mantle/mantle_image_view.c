#include "mantle_internal.h"

// Image View Functions

GR_RESULT grCreateColorTargetView(
    GR_DEVICE device,
    const GR_COLOR_TARGET_VIEW_CREATE_INFO* pCreateInfo,
    GR_COLOR_TARGET_VIEW* pView)
{
    LOGT("%p %p %p\n", device, pCreateInfo, pView);
    GrDevice* grDevice = (GrDevice*)device;
    GrImage* grImage = (GrImage*)pCreateInfo->image;
    VkImageView vkImageView = VK_NULL_HANDLE;

    // TODO validate parameters

    const VkImageViewCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .image = grImage->image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = getVkFormat(pCreateInfo->format),
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY,
        },
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = pCreateInfo->mipLevel,
            .levelCount = 1,
            .baseArrayLayer = pCreateInfo->baseArraySlice,
            .layerCount = pCreateInfo->arraySize,
        }
    };

    VkResult res = vki.vkCreateImageView(grDevice->device, &createInfo, NULL, &vkImageView);
    if (res != VK_SUCCESS) {
        LOGE("vkCreateImageView failed (%d)\n", res);
        return getGrResult(res);
    }

    GrColorTargetView* grColorTargetView = malloc(sizeof(GrColorTargetView));
    *grColorTargetView = (GrColorTargetView) {
        .sType = GR_STRUCT_TYPE_COLOR_TARGET_VIEW,
        .imageView = vkImageView,
        .extent2D = grImage->extent,
        .layerCount = pCreateInfo->arraySize,
    };

    *pView = (GR_COLOR_TARGET_VIEW)grColorTargetView;
    return GR_SUCCESS;
}
