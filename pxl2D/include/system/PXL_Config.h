#ifndef _PXL_CONFIG_H
#define _PXL_CONFIG_H

//exception config
#define PXL_SHOW_EXCEPTIONS_IN_DEBUG 1 /**< Defines whether exceptions will show in debug mode or not **/
#define PXL_SHOW_EXCEPTIONS_IN_RELEASE 0 /**< Defines whether exceptions will show in release mode or not **/
#define PXL_EXCEPTION_TITLE "Error" /**< The error title used when logged **/

//error codes
typedef std::string PXL_ErrorCode;

#define PXL_ERROR_NONE ""
#define PXL_ERROR_UNKNOWN "UNKNOWN"
#define PXL_ERROR_SWAP_BUFFERS_FAILED "SWAP_BUFFERS_FAILED"
#define PXL_ERROR_MISSING_SHADER_HEADERS "MISSING_SHADER_HEADERS"
#define PXL_ERROR_INVALID_FILE "INVALID_FILE"
#define PXL_ERROR_EMPTY_FILE "EMPTY_FILE"
#define PXL_ERROR_SHADER_LINK_FAILED "SHADER_LINK_FAILED"
#define PXL_ERROR_SHADER_COMPILE_FAILED "SHADER_COMPILE_FAILED"
#define PXL_ERROR_INVALID_PNG "INVALID_PNG"
#define PXL_ERROR_TEXTURE_CREATION_FAILED "TEXTURE_CREATION_FAILED"

#endif