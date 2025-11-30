#include "ee.h"  // Include the EEPROM emulation header
#include "eeConfig.h"  // Include configuration for EEPROM emulation
#include <string.h>  // Include string library for memcpy and memset
#include "stm32f1xx.h"  // Include CMSIS header for FLASH register definitions

#define   PAGE                  0  // Define PAGE constant for page mode
#define   SECTOR                1  // Define SECTOR constant for sector mode
#define   PAGE_NUM              2  // Define PAGE_NUM constant for page number mode

#if defined(STM32F103xB)  // Conditional compilation for STM32F103xB MCU
#define   _EE_SIZE              1024  // Define EEPROM size as 1024 bytes
#define   _EE_ADDR_INUSE        (((uint32_t)0x08000000) | (_EE_SIZE * _EE_USE_FLASH_PAGE_OR_SECTOR))  // Calculate the flash address in use
#define   _EE_FLASH_BANK        FLASH_BANK_1  // Define flash bank as bank 1
#define   _EE_PAGE_OR_SECTOR    PAGE  // Set page or sector mode to PAGE
#if (_EE_USE_FLASH_PAGE_OR_SECTOR > 127)  // Check if page number exceeds maximum
#error  "Please Enter correct address, maximum is (127)"
#endif
#endif

#if defined(STM32F103x8)  // Conditional compilation for STM32F103x8 MCU
#define   _EE_SIZE          	1024  // Define EEPROM size as 1024 bytes
#define   _EE_ADDR_INUSE        (((uint32_t)0x08000000) | (_EE_SIZE * _EE_USE_FLASH_PAGE_OR_SECTOR))  // Calculate the flash address in use
#define   _EE_FLASH_BANK        FLASH_BANK_1  // Define flash bank as bank 1
#define   _EE_PAGE_OR_SECTOR    PAGE  // Set page or sector mode to PAGE
#if (_EE_USE_FLASH_PAGE_OR_SECTOR > 63)  // Check if page number exceeds maximum
#error  "Please Enter correct address, maximum is (63)"
#endif
#endif

#if defined(STM32F103xC)  // Conditional compilation for STM32F103xC MCU
#define   _EE_SIZE              2048  // Define EEPROM size as 2048 bytes
#define   _EE_ADDR_INUSE        (((uint32_t)0x08000000) | (_EE_SIZE * _EE_USE_FLASH_PAGE_OR_SECTOR))  // Calculate the flash address in use
#define   _EE_FLASH_BANK        FLASH_BANK_1  // Define flash bank as bank 1
#define   _EE_PAGE_OR_SECTOR    PAGE  // Set page or sector mode to PAGE
#if (_EE_USE_FLASH_PAGE_OR_SECTOR > 127)  // Check if page number exceeds maximum
#error  "Please Enter correct address, maximum is (127)"
#endif
#endif

#if defined(STM32F103xD)  // Conditional compilation for STM32F103xD MCU
#define   _EE_SIZE              2048  // Define EEPROM size as 2048 bytes
#define   _EE_ADDR_INUSE        (((uint32_t)0x08000000) | (_EE_SIZE * _EE_USE_FLASH_PAGE_OR_SECTOR))  // Calculate the flash address in use
#define   _EE_FLASH_BANK        FLASH_BANK_1  // Define flash bank as bank 1
#define   _EE_PAGE_OR_SECTOR    PAGE  // Set page or sector mode to PAGE
#if (_EE_USE_FLASH_PAGE_OR_SECTOR > 191)  // Check if page number exceeds maximum
#error  "Please Enter correct address, maximum is (191)"
#endif
#endif

#if defined(STM32F103xE)  // Conditional compilation for STM32F103xE MCU
#define   _EE_SIZE              2048  // Define EEPROM size as 2048 bytes
#define   _EE_ADDR_INUSE        (((uint32_t)0x08000000) | (_EE_SIZE * _EE_USE_FLASH_PAGE_OR_SECTOR))  // Calculate the flash address in use
#define   _EE_FLASH_BANK        FLASH_BANK_1  // Define flash bank as bank 1
#define   _EE_PAGE_OR_SECTOR    PAGE  // Set page or sector mode to PAGE
#if (_EE_USE_FLASH_PAGE_OR_SECTOR > 255)  // Check if page number exceeds maximum
#error  "Please Enter correct address, maximum is (255)"
#endif
#endif

#if defined(STM32F030x4) || defined(STM32F042x4)  || defined(STM32F070x4)  // Conditional compilation for specified MCUs
#define   _EE_SIZE              1024  // Define EEPROM size as 1024 bytes
#define   _EE_ADDR_INUSE        (((uint32_t)0x08000000) | (_EE_SIZE * _EE_USE_FLASH_PAGE_OR_SECTOR))  // Calculate the flash address in use
#define   _EE_PAGE_OR_SECTOR    PAGE  // Set page or sector mode to PAGE
#if (_EE_USE_FLASH_PAGE_OR_SECTOR > 15)  // Check if page number exceeds maximum
#error  "Please Enter correct address, maximum is (15)"
#endif
#endif

#if  defined(STM32F030x6) || defined(STM32F042x6) || defined(STM32F070x6)  // Conditional compilation for specified MCUs
#define   _EE_SIZE              1024  // Define EEPROM size as 1024 bytes
#define   _EE_ADDR_INUSE        (((uint32_t)0x08000000) | (_EE_SIZE * _EE_USE_FLASH_PAGE_OR_SECTOR))  // Calculate the flash address in use
#define   _EE_PAGE_OR_SECTOR    PAGE  // Set page or sector mode to PAGE
#if (_EE_USE_FLASH_PAGE_OR_SECTOR > 31)  // Check if page number exceeds maximum
#error  "Please Enter correct address, maximum is (31)"
#endif
#endif

#if defined(STM32F030x8) || defined(STM32F042x8)  // Conditional compilation for specified MCUs
#define   _EE_SIZE              1024  // Define EEPROM size as 1024 bytes
#define   _EE_ADDR_INUSE        (((uint32_t)0x08000000) | (_EE_SIZE * _EE_USE_FLASH_PAGE_OR_SECTOR))  // Calculate the flash address in use
#define   _EE_PAGE_OR_SECTOR    PAGE  // Set page or sector mode to PAGE
#if (_EE_USE_FLASH_PAGE_OR_SECTOR > 63)  // Check if page number exceeds maximum
#error  "Please Enter correct address, maximum is (63)"
#endif
#endif

#if defined(STM32F070xB)  // Conditional compilation for STM32F070xB MCU
#define   _EE_SIZE              2048  // Define EEPROM size as 2048 bytes
#define   _EE_ADDR_INUSE        (((uint32_t)0x08000000) | (_EE_SIZE * _EE_USE_FLASH_PAGE_OR_SECTOR))  // Calculate the flash address in use
#define   _EE_PAGE_OR_SECTOR    PAGE  // Set page or sector mode to PAGE
#if (_EE_USE_FLASH_PAGE_OR_SECTOR > 63)  // Check if page number exceeds maximum
#error  "Please Enter correct address, maximum is (63)"
#endif
#endif

#if defined(STM32F070xC)  // Conditional compilation for STM32F070xC MCU
#define   _EE_SIZE              2048  // Define EEPROM size as 2048 bytes
#define   _EE_ADDR_INUSE        (((uint32_t)0x08000000) | (_EE_SIZE * _EE_USE_FLASH_PAGE_OR_SECTOR))  // Calculate the flash address in use
#define   _EE_PAGE_OR_SECTOR    PAGE  // Set page or sector mode to PAGE
#if (_EE_USE_FLASH_PAGE_OR_SECTOR > 127)  // Check if page number exceeds maximum
#error  "Please Enter correct address, maximum is (127)"
#endif
#endif

#if defined(STM32F405xx) || defined(STM32F407xx) || defined(STM32F415xx) || defined(STM32F417xx)  // Conditional compilation for specified MCUs
#define   _EE_SIZE              (1024 * 128)  // Define EEPROM size as 128KB
#define   _EE_ADDR_INUSE        (((uint32_t)0x08020000) | (_EE_SIZE*(_EE_USE_FLASH_PAGE_OR_SECTOR - 5)))  // Calculate the flash address in use
#define   _EE_FLASH_BANK        FLASH_BANK_1  // Define flash bank as bank 1
#define   _EE_VOLTAGE_RANGE     _EE_VOLTAGE  // Define voltage range
#define   _EE_PAGE_OR_SECTOR    SECTOR  // Set page or sector mode to SECTOR
#if (_EE_USE_FLASH_PAGE_OR_SECTOR > 11)  // Check if sector number exceeds maximum
#error  "Please Enter correct address, maximum is (11)"
#endif
#if (_EE_USE_FLASH_PAGE_OR_SECTOR < 5)  // Check if sector number is below minimum
#error  "Please Enter correct address, minimum is (5)"
#endif
#endif

#if (_EE_USE_RAM_BYTE > 0)  // If RAM byte usage is enabled
uint8_t ee_ram[_EE_USE_RAM_BYTE];  // Declare RAM array for EEPROM emulation
#endif

// Khởi tạo EEPROM emulation (thay HAL_FLASH_Unlock bằng bare-metal)
bool ee_init(void)
{
  // Unlock Flash
  FLASH->KEYR = FLASH_KEY1;  // Write first key to unlock flash
  FLASH->KEYR = FLASH_KEY2;  // Write second key to unlock flash

  // Kiểm tra và format nếu cần (giữ logic gốc)
  uint8_t temp[_EE_SIZE];  // Declare temporary array to read flash content
  ee_read(0, _EE_SIZE, temp);  // Read the entire EEPROM size into temp
  for (uint16_t i = 0; i < _EE_SIZE; i++)  // Loop through the temp array
  {
    if (temp[i] != 0xFF)  // If any byte is not 0xFF (erased state)
      return true;  // Return true indicating data exists
  }
  return ee_format(false);  // If all erased, format without keeping RAM data
}

// Format Flash (thay HAL_FLASHEx_Erase)
bool ee_format(bool keepRamData)
{
  // Unlock Flash
  FLASH->KEYR = FLASH_KEY1;  // Write first key to unlock flash
  FLASH->KEYR = FLASH_KEY2;  // Write second key to unlock flash

  // Erase page/sector
  FLASH->CR |= FLASH_CR_PER;  // Set page erase bit in control register
  FLASH->AR = _EE_ADDR_INUSE;  // Set address register to the page address
  FLASH->CR |= FLASH_CR_STRT;  // Start the erase operation

  // Chờ erase hoàn thành với timeout
  uint32_t timeout = 1000000;  // Set timeout counter to 1 million
  while ((FLASH->SR & FLASH_SR_BSY) && timeout--) {}  // Wait while busy flag is set or timeout
  if (timeout == 0) return false;  // Return false if timeout occurred

  FLASH->CR &= ~FLASH_CR_PER;  // Clear page erase bit

  // Kiểm tra lỗi
  if (FLASH->SR & FLASH_SR_EOP) {  // If end of operation flag is set
    FLASH->SR |= FLASH_SR_EOP;  // Clear end of operation flag
  } else {
    return false;  // Return false if no EOP flag (error)
  }

  // Lock Flash lại
  FLASH->CR |= FLASH_CR_LOCK;  // Lock flash by setting lock bit

  #if (_EE_USE_RAM_BYTE > 0)  // If RAM usage is enabled
  if (keepRamData == false)  // If not keeping RAM data
    memset(ee_ram, 0xFF, _EE_USE_RAM_BYTE);  // Set RAM to erased state (0xFF)
  #endif
  return true;  // Return true on success
}

// Đọc dữ liệu (bare-metal, đọc trực tiếp từ Flash)
bool ee_read(uint32_t startVirtualAddress, uint32_t len, uint8_t* data)
{
  if ((startVirtualAddress + len) > _EE_SIZE)  // Check if address + length exceeds EEPROM size
    return false;  // Return false if out of bounds
  for (uint32_t i = startVirtualAddress; i < len + startVirtualAddress; i++)  // Loop through the range
  {
    if (data != NULL)  // If data pointer is not null
    {
      *data = (*(__IO uint8_t*) (i + _EE_ADDR_INUSE));  // Read byte from flash and store in data
      data++;  // Increment data pointer
    }
#if (_EE_USE_RAM_BYTE > 0)  // If RAM usage is enabled
    if (i < _EE_USE_RAM_BYTE)  // If within RAM byte range
      ee_ram[i] = (*(__IO uint8_t*) (i + _EE_ADDR_INUSE));  // Copy to RAM array
#endif
  }
  return true;  // Return true on success
}

// Ghi dữ liệu (thay HAL_FLASH_Program bằng bare-metal)
bool ee_write(uint32_t startVirtualAddress, uint32_t len, uint8_t *data)
{
  if ((startVirtualAddress + len) > _EE_SIZE)  // Check if address + length exceeds EEPROM size
    return false;  // Return false if out of bounds
  if (data == NULL)  // Check if data pointer is null
    return false;  // Return false if null

  // Unlock Flash
  FLASH->KEYR = FLASH_KEY1;  // Write first key to unlock flash
  FLASH->KEYR = FLASH_KEY2;  // Write second key to unlock flash

#ifdef FLASH_TYPEPROGRAM_BYTE  // If byte programming is defined
  for (uint32_t i = 0; i < len ; i++)  // Loop through length
  {		
    FLASH->CR |= FLASH_CR_PG;  // Set programming bit
    *(__IO uint8_t*)(_EE_ADDR_INUSE + startVirtualAddress + i) = data[i];  // Write byte to flash

    // Chờ hoàn thành với timeout
    uint32_t timeout = 1000000;  // Set timeout counter
    while ((FLASH->SR & FLASH_SR_BSY) && timeout--) {}  // Wait while busy
    if (timeout == 0) {  // If timeout
      FLASH->CR |= FLASH_CR_LOCK;  // Lock flash
      return false;  // Return false
    }

    if (FLASH->SR & FLASH_SR_EOP) {  // If end of operation
      FLASH->SR |= FLASH_SR_EOP;  // Clear EOP flag
    } else {  // If error
      FLASH->CR |= FLASH_CR_LOCK;  // Lock flash
      return false;  // Return false
    }
    FLASH->CR &= ~FLASH_CR_PG;  // Clear programming bit
  }	
#endif
#ifdef FLASH_TYPEPROGRAM_HALFWORD  // If halfword programming is defined
  for (uint32_t i = 0; i < len ; i+=2)  // Loop through length in steps of 2
  {		
    FLASH->CR |= FLASH_CR_PG;  // Set programming bit
    *(__IO uint16_t*)(_EE_ADDR_INUSE + startVirtualAddress + i) = (uint16_t)(data[i] | (data[i+1] << 8));  // Write halfword to flash

    // Chờ hoàn thành với timeout
    uint32_t timeout = 1000000;  // Set timeout counter
    while ((FLASH->SR & FLASH_SR_BSY) && timeout--) {}  // Wait while busy
    if (timeout == 0) {  // If timeout
      FLASH->CR |= FLASH_CR_LOCK;  // Lock flash
      return false;  // Return false
    }

    if (FLASH->SR & FLASH_SR_EOP) {  // If end of operation
      FLASH->SR |= FLASH_SR_EOP;  // Clear EOP flag
    } else {  // If error
      FLASH->CR |= FLASH_CR_LOCK;  // Lock flash
      return false;  // Return false
    }
    FLASH->CR &= ~FLASH_CR_PG;  // Clear programming bit
  }	
#endif
#ifdef FLASH_TYPEPROGRAM_DOUBLEWORD  // If doubleword programming is defined
  for (uint32_t i = 0; i < len; i += 8)  // Loop through length in steps of 8
  {
    uint64_t data64 = data[i];  // Initialize 64-bit data with first byte
    data64 += data[i + 1] * 0x100;  // Add second byte shifted
    data64 += data[i + 2] * 0x10000;  // Add third byte shifted
    data64 += data[i + 3] * 0x1000000;  // Add fourth byte shifted
    data64 += data[i + 4] * 0x100000000;  // Add fifth byte shifted
    data64 += data[i + 5] * 0x10000000000;  // Add sixth byte shifted
    data64 += data[i + 6] * 0x1000000000000;  // Add seventh byte shifted
    data64 += data[i + 7] * 0x100000000000000;  // Add eighth byte shifted

    FLASH->CR |= FLASH_CR_PG;  // Set programming bit
    *(__IO uint64_t*)(_EE_ADDR_INUSE + startVirtualAddress + i) = data64;  // Write doubleword to flash

    // Chờ hoàn thành với timeout
    uint32_t timeout = 1000000;  // Set timeout counter
    while ((FLASH->SR & FLASH_SR_BSY) && timeout--) {}  // Wait while busy
    if (timeout == 0) {  // If timeout
      FLASH->CR |= FLASH_CR_LOCK;  // Lock flash
      return false;  // Return false
    }

    if (FLASH->SR & FLASH_SR_EOP) {  // If end of operation
      FLASH->SR |= FLASH_SR_EOP;  // Clear EOP flag
    } else {  // If error
      FLASH->CR |= FLASH_CR_LOCK;  // Lock flash
      return false;  // Return false
    }
    FLASH->CR &= ~FLASH_CR_PG;  // Clear programming bit
  }
#endif
  // Lock Flash
  FLASH->CR |= FLASH_CR_LOCK;  // Lock flash by setting lock bit
  return true;  // Return true on success
}

//##########################################################################################################
bool ee_writeToRam(uint32_t startVirtualAddress, uint32_t len, uint8_t* data)
{
#if (_EE_USE_RAM_BYTE > 0)  // If RAM usage is enabled
  if ((startVirtualAddress + len) > _EE_USE_RAM_BYTE)  // Check if address + length exceeds RAM size
    return false;  // Return false if out of bounds
  if (data == NULL)  // Check if data pointer is null
    return false;  // Return false if null
  memcpy(&ee_ram[startVirtualAddress], data, len);  // Copy data to RAM array
  return true;  // Return true on success
#else
  return false;  // Return false if RAM not enabled
#endif
}
//##########################################################################################################
bool  ee_commit(void)
{
#if (_EE_USE_RAM_BYTE > 0)  // If RAM usage is enabled
  if (ee_format(true) == false)  // Format flash keeping RAM data
    return false;  // Return false if format fails
  return ee_write(0, _EE_USE_RAM_BYTE, ee_ram);  // Write RAM to flash
#else
  return false;  // Return false if RAM not enabled
#endif
}
//##########################################################################################################
uint32_t  ee_maxVirtualAddress(void)
{
  return (_EE_SIZE);  // Return the maximum virtual address (EEPROM size)  
}
//##########################################################################################################