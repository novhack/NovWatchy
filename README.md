# NovWatchy

Arduino/PlatformIO based firmware for Watchy 2.0 based on the official Watchy firmware (https://github.com/sqfmi/Watchy) and inspired by My-TTGO-Watch project (https://github.com/sharandac/My-TTGO-Watch).

This firmware is tailored to fit my specific needs and requirements.

Changes done:

- Completely restructured and refactored base code
- Added Timer app

(TODO)
- Added automatic daily NTP sync through WiFi
- Added hourly RTC time adjustment 
    - In my case RTC in Watchy can run a bit too fast (around 14 seconds a day)
    - The adjustment amount is recalculated with every daily NTP sync
- Added more detailed battery status
    - Voltage representing 100% recalculated with every full charge
    - Minimum voltage experimentally measured as 3.3V
    - Added time estimates for charging and discharging
