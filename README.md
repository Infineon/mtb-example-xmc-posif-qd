# XMC MCU: POSIF QD

This example demonstrates a Position Interface (POSIF) module in Quadrature Decoder (QD) mode and uses the CCU40 module to determine the speed of rotation of the encoder. LED1 turns ON when the encoder rotates in clockwise direction; LED2 turns ON when the rotation is anti-clockwise.

## Requirements

- [ModusToolbox&trade; software](https://www.cypress.com/products/modustoolbox-software-environment) v2.3
- [SEGGER J-Link software](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack)
- Programming Language: C
- Associated Parts: All [XMC™ MCU](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/) parts

## Supported Toolchains (make variable 'TOOLCHAIN')

- GNU Arm® Embedded Compiler v9.3.1 (`GCC_ARM`) - Default value of `TOOLCHAIN`

## Supported Kits (make variable 'TARGET')

- [XMC1400 Boot Kit](https://www.infineon.com/cms/en/product/evaluation-boards/kit_xmc14_boot_001/) (`KIT_XMC14_BOOT_001`) - Default value of `TARGET`
- [XMC4700 Relax Kit](https://www.infineon.com/cms/en/product/evaluation-boards/kit_xmc47_relax_v1/) (`KIT_XMC47_RELAX_V1`)

## Hardware Setup

This example uses the board's default configuration. See the kit user guide to ensure that the board is configured correctly.

Table 1 and 2 show the input signal connections from the encoder to ports in XMC1400 and XMC4700 kits.

**Table 1. Input Connections in XMC1400 Boot Kit**

|            Input signal       |    Ports     |
|-----------------------------------|------------------|
|         Phase A Input signal      |    Port P0.13    |
|         Phase B Input signal      |    Port P1.1     |
|             Index signal          |    Port P1.0     |

**Table 2. Input Connections in XMC4700 Relax Kit**

|            Input signal       |    Ports     |
|-----------------------------------|------------------|
|         Phase A Input signal      |    Port P14.7    |
|         Phase B Input signal      |    Port P14.6    |
|             Index signal          |    Port P1.1     |

For XMC 1400 Boot Kit, connect P1.0 (Index signal) to P0.7 (CCU40.CC41/Event0 input signal).

## Software Setup

Install a terminal emulator if you don't have one. Instructions in this document use [Tera Term](https://ttssh2.osdn.jp/index.html.en).

## Using the Code Example

Create the project and open it using one of the following:

<details open><summary><b>In Eclipse IDE for ModusToolbox</b></summary>

1. Click the **New Application** link in the **Quick Panel** (or, use **File** > **New** > **ModusToolbox Application**). This launches the [Project Creator](http://www.cypress.com/ModusToolboxProjectCreator) tool.

2. Pick a kit supported by the code example from the list shown in the **Project Creator - Choose Board Support Package (BSP)** dialog.

   When you select a supported kit, the example is reconfigured automatically to work with the kit. To work with a different supported kit later, use the [Library Manager](https://www.cypress.com/ModusToolboxLibraryManager) to choose the BSP for the supported kit. You can use the Library Manager to select or update the BSP and firmware libraries used in this application. To access the Library Manager, click the link from the **Quick Panel**.

   You can also just start the application creation process again and select a different kit.

   If you want to use the application for a kit not listed here, you may need to update the source files. If the kit does not have the required resources, the application may not work.

3. In the **Project Creator - Select Application** dialog, choose the example by enabling the checkbox.

4. Optionally, change the suggested **New Application Name**.

5. Enter the local path in the **Application(s) Root Path** field to indicate where the application needs to be created.

   Applications that can share libraries can be placed in the same root path.

6. Click **Create** to complete the application creation process.

For more details, see the [Eclipse IDE for ModusToolbox User Guide](https://www.cypress.com/MTBEclipseIDEUserGuide) (locally available at *{ModusToolbox install directory}/ide_{version}/docs/mt_ide_user_guide.pdf*).

</details>

<details open><summary><b>In Command-line Interface (CLI)</b></summary>

ModusToolbox provides the Project Creator as both a GUI tool and a command line tool to easily create one or more ModusToolbox applications. See the "Project Creator Tools" section of the [ModusToolbox User Guide](https://www.cypress.com/ModusToolboxUserGuide) for more details.

Alternatively, you can manually create the application using the following steps:

1. Download and unzip this repository onto your local machine, or clone the repository.

2. Open a CLI terminal and navigate to the application folder.

   On Linux and macOS, you can use any terminal application. On Windows, open the **modus-shell** app from the Start menu.

   **Note:** The cloned application contains a default BSP file (*TARGET_xxx.mtb*) in the *deps* folder. Use the [Library Manager](https://www.cypress.com/ModusToolboxLibraryManager) (`make modlibs` command) to select and download a different BSP file, if required. If the selected kit does not have the required resources or is not [supported](#supported-kits-make-variable-target), the application may not work.

3. Import the required libraries by executing the `make getlibs` command.

Various CLI tools include a `-h` option that prints help information to the terminal screen about that tool. For more details, see the [ModusToolbox User Guide](https://www.cypress.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>

<details open><summary><b>In Third-party IDEs</b></summary>

**Note:** Only VS Code is supported.

1. Follow the instructions from the **In Command-line Interface (CLI)** section to create the application, and import the libraries using the `make getlibs` command.

2. Export the application to a supported IDE using the `make <ide>` command.

   For a list of supported IDEs and more details, see the "Exporting to IDEs" section of the [ModusToolbox User Guide](https://www.cypress.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox install directory}/docs_{version}/mtb_user_guide.pdf*).

3. Follow the instructions displayed in the terminal to create or import the application as an IDE project.

</details>

## Operation

1. Connect the board to your PC using a micro-USB cable through the debug USB connector.

2. Open a terminal program and select the KitProg3 COM port. Set the serial port parameters to 8N1 and 115200 baud.

3. Program the board using Eclipse IDE for ModusToolbox:

   1. Select the application project in the Project Explorer.

   2. In the **Quick Panel**, scroll down, and click **\<Application Name> Program (JLink)**.

4. Connect Phase A, Phase B and Index Signals from external encoder to posif module input pins as mentioned in hardware setup section.

3. Confirm the operation of the example:

   1. Confirm that USER_LED1 turns ON when the encoder rotates in the clockwise direction.

   2. Confirm that USER_LED2 turns ON when the motor encoder rotates in the anti-clockwise direction.

4. Confirm that index signal time period is displayed on the UART terminal as shown below.

**Figure 1. Terminal output**

![](images/tera-term.jpg)

<br>

## Debugging

You can debug the example to step through the code. In the IDE, use the **\<Application Name> Debug (JLink)** configuration in the **Quick Panel**. For more details, see the "Program and Debug" section in the [Eclipse IDE for ModusToolbox User Guide](https://www.cypress.com/MTBEclipseIDEUserGuide).

## Design and Implementation

The application uses the 'CYBSP_DEBUG_UART' resource to print messages in a UART terminal emulator. This resource is configured by the ModusToolbox UART personality. The retargeting of the standard I/O to the CYBSP_DEBUG_UART port is included in the example. After using `retarget_io_init`, messages can be printed on the terminal by simply using `printf` commands.

The POSIF module is configured in Quadrature Decoder mode. Within this mode, standard mode is selected, which is used when the external rotary encoder provides two phase signals and additionally an index signal that is generated once per shaft revolution. Encoder signals are connected to the POSIF module input ports. The POSIF0.OUT1 pin from the POSIF module is asserted HIGH when the encoder is rotating clockwise and LOW when it is rotating in anti-clockwise direction.

The application uses a CCU4 slice configured using the CCU4 personality. The CAPTURE_0 CCU4 slice is configured in capture mode. It will capture the timer value on the rising edge of the index signal.

In the main loop, the direction is continuously monitored; USER_LED1 turns ON when motor rotates in clock wise direction and USER_LED2 turns ON when motor rotates in anti-clockwise direction. It also checks for the occurrence of the rising edge event of the index signal, reads out the captured timer, and prints the index signal time period on terminal.

### Resources and Settings

The project uses a custom *design.modus* file because the following settings were modified in the default *design.modus* file.

**Figure 2. CCU40 Configuration for XMC1400 Boot Kit**

![](images/ccu40.jpg)

<br>

**Figure 3. CCU40 Configuration for XMC4700 Relax Kit**

![](images/ccu40-47x.jpg)

<br>

**Figure 4. Phase_A Pin Settings for XMC1400 Boot Kit**

![](images/phase-a-14x.jpg)

<br>

**Figure 5. Phase_B Pin Settings for XMC1400 Boot Kit**

![](images/phase-b-14x.jpg)

<br>

**Figure 6. Index Signal Pin Settings for XMC1400 Boot Kit**

![](images/index-signal-14x.jpg)

<br>

**Figure 7. P0.7 Pin Settings for XMC1400 Boot Kit**

![](images/p0.7.jpg)

<br>

**Figure 8. Phase_A Pin Settings for XMC4700 Relax Kit**

![](images/phase-a-47x.jpg)

<br>

**Figure 9. Phase_B Pin Settings for XMC4700 Relax Kit**

![](images/phase-b-47x.jpg)

<br>

**Figure 10. Index Signal Pin Settings for XMC4700 Relax Kit**

![](images/index-signal-47x.jpg)

<br>

## Related Resources

| Kit Guides                                            |                                                              |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| [XMC4700/XMC4800 Relax Kit Series-V1](https://www.infineon.com/dgdl/Infineon-Board_User_Manual_XMC4700_XMC4800_Relax_Kit_Series-UM-v01_02-EN.pdf?fileId=5546d46250cc1fdf01513f8e052d07fc) – Board User‘s Manual | Describes the schematic and hardware of the XMC4700/XMC4800 Relax Kit Series-V1, equipped with an Arm® Cortex®-M4 based XMC™ Microcontroller from Infineon Technologies AG. |
| [XMC1400 Boot Kit](https://www.infineon.com/dgdl/Infineon-Board_Users_Manual_XMC1400_Boot_Kit.pdf-UM-v01_00-EN.pdf?fileId=5546d462525dbac401527815f9a073fd) – Board User‘s Manual | Describes the schematic and hardware of XMC1400 Boot Kit for application code development on the XMC1404-Q064X0200 device.|
| **Code Examples**                                            |                                                              |
| [Using ModusToolbox](https://github.com/cypresssemiconductorco/Code-Examples-for-ModusToolbox-Software) |
| **Device Documentation**                                     |                                                              |
| [XMC1000 Family Datasheets](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/32-bit-xmc1000-industrial-microcontroller-arm-cortex-m0/#document-group-myInfineon-49) | [XMC1000 Family Technical Reference Manuals](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/32-bit-xmc1000-industrial-microcontroller-arm-cortex-m0/#document-group-myInfineon-44) |
| [XMC4000 Family Datasheets](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/32-bit-xmc4000-industrial-microcontroller-arm-cortex-m4/#document-group-myInfineon-49) | [XMC4000 Family Technical Reference Manuals](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/32-bit-xmc4000-industrial-microcontroller-arm-cortex-m4/#document-group-myInfineon-44) |
| [AP32289 Position Interface (POSIF)](https://www.infineon.com/dgdl/Infineon-POSIF-XMC1000_XMC4000-AP32289-AN-v01_00-EN.pdf?fileId=5546d4624e765da5014ed938a6a7311f) |
| **Development Kits**                                         | Buy at www.infineon.com                                       |
| [KIT_XMC14_BOOT_001](https://www.infineon.com/cms/en/product/evaluation-boards/kit_xmc14_boot_001/) Boot Kit XMC1400 | [KIT_XMC47_RELAX_V1](https://www.infineon.com/cms/en/product/evaluation-boards/kit_xmc47_relax_v1/) XMC4700 Relax Kit |
| **Libraries**                                                 |                                                              |
| XMC Peripheral Library (XMCLib) and docs  | [mtb-xmclib-cat3](https://github.com/cypresssemiconductorco/mtb-xmclib-cat3) on GitHub |
| **Tools**                                                    |                                                              |
| [Eclipse IDE for ModusToolbox](https://www.cypress.com/modustoolbox)     | The cross-platform, Eclipse-based IDE for IoT designers that supports application configuration and development targeting converged MCU and wireless systems.             |

## Other Resources

Infineon provides a wealth of data at www.infineon.com to help you select the right device, and quickly and effectively integrate it into your design.

For XMC MCU devices, see [32-bit XMC™ Industrial Microcontroller based on Arm® Cortex®-M](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/).

## Document History

Document Title: *CE232707* - *XMC MCU: POSIF QD*

| Version | Description of Change |
| ------- | --------------------- |
| 1.0.0   | New code example      |
------

All other trademarks or registered trademarks referenced herein are the property of their respective owners.

![banner](images/ifx_logo_rgb.jpg)

© 2021 Infineon Technologies AG

All Rights Reserved.

### Legal Disclaimer

The information given in this document shall in no event be regarded as a guarantee of conditions or characteristics. With respect to any examples or hints given herein, any typical values stated herein and/or any information regarding the application of the device, Infineon Technologies hereby disclaims any and all warranties and liabilities of any kind, including without limitation, warranties of non-infringement of intellectual property rights of any third party.

### Information

For further information on technology, delivery terms and conditions and prices, please contact the nearest Infineon Technologies Office (www.infineon.com).

### Warnings

Due to technical requirements, components may contain dangerous substances. For information on the types in question, please contact the nearest Infineon Technologies Office.

Infineon Technologies components may be used in life-support devices or systems only with the express written approval of Infineon Technologies, if a failure of such components can reasonably be expected to cause the failure of that life-support device or system or to affect the safety or effectiveness of that device or system. Life support devices or systems are intended to be implanted in the human body or to support and/or maintain and sustain and/or protect human life. If they fail, it is reasonable to assume that the health of the user or other persons may be endangered.

-------------------------------------------------------------------------------
