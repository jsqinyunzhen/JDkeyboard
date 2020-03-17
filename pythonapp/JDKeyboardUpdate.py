import os

import serial
import wx
import threading, time
from binascii import *
import crcmod
#from PyCRC.CRC16 import CRC16
#from crcmod import *

textPortNum = None
ser = None
bserisopen = False
bupdate = False
protext = None
bsendbin = False
STRGLO = None
filename = None
filedata = None
file = None
f1 =None
wait1second = b'wait 1 Second\r\n'
rxstr = ''
devicestatus = None

def crc16(x, invert):
    a = 0xFFFF
    b = 0xA001
    for byte in x:
        a ^= ord(byte)
        for i in range(8):
            last = a % 2
            a >>= 1
            if last == 1:
                a ^= b
    s = hex(a).upper()

    return s[4:6] + s[2:4] if invert == True else s[2:4] + s[4:6]


def crc16Add(read):
    crc16 =crcmod.mkCrcFun(0x18005,rev=True,initCrc=0xFFFF,xorOut=0x0000)
    data = read.replace(" ","")
    readcrcout=hex(crc16(unhexlify(data))).upper()
    str_list = list(readcrcout)
    if len(str_list) == 5:
        str_list.insert(2,'0')      # 位数不足补0
    crc_data = "".join(str_list)
    print(crc_data)
    read = read.strip()+' '+crc_data[4:]+' '+crc_data[2:4]
    print('CRC16校验:',crc_data[4:]+' '+crc_data[2:4])
    print('增加Modbus CRC16校验：>>>',read)

def crc16Addbytes(data):
    crc16 = crcmod.mkCrcFun(0x18005, rev=True, initCrc=0xFFFF, xorOut=0x0000)
    readcrcout = crc16(data)
    return readcrcout
    print(type(readcrcout))
    print(type(data))
    data = data + readcrcout.to_bytes(length=2,byteorder='little',signed=False)
    print('crc = ',hex(readcrcout))
    print('crc = ', data, len(data))

def openbinfile(path,bshowerror = 0):
    global filedata
    global file
    try:
        file = open(path, 'rb')
        filedata = file.read()

        if not filedata or len(filedata) == 0:
            if bshowerror:
                box = wx.MessageDialog(None, '升级文件错误！', '错误', wx.OK)
                box.ShowModal()
                box.Destroy()
            filedata = None
            file.close()
            return

        protext.AppendText(filedata.hex().upper())
        # protext.AppendText(filedata.decode('GBK'))
        crc = crc16Addbytes(filedata)
        protext.AppendText("\r\ncrc=" + hex(crc)+"\r\n")
        filedata = filedata + crc.to_bytes(length=2, byteorder='little', signed=False)

        # print('data = ', filedata, len(filedata))
        # print('crc = ', hex(crc))
        # crc = crc16Addbytes(filedata)
        # print('crc = ', crc)
        file.close()

    except Exception as e:
        print("----异常：", e)
        filedata = None
        if bshowerror:
            box = wx.MessageDialog(None,'打开升级文件错误！', '错误', wx.OK)
            box.ShowModal()
            box.Destroy()

    except FileNotFoundError:
        print('无法打开指定的文件!')

    except LookupError:
        print('指定了未知的编码!')
    except UnicodeDecodeError:
        print('读取文件时解码错误!')

    finally:
        if file:
            file.close()

def savesettingfile():
    global f1
    global filename
    global textPortNum
    try:
        f1 = open("setting.txt", 'w+')
        str = textPortNum.GetValue()
        str += '\n'
        f1.write(str)
        f1.write(filename.GetValue() + '\n')
        f1.close()

    except Exception as e:
        print("----异常：", e)
    except FileNotFoundError:
        print('无法打开指定的文件!')
    except LookupError:
        print('指定了未知的编码!')
    except UnicodeDecodeError:
        print('读取文件时解码错误!')
    finally:
        if f1:
            f1.close()

def opensettingfile():
    global f1
    global filename
    global textPortNum
    try:
        f1 = open("setting.txt", 'r')
        data = f1.read().splitlines()
        print(data[0])
        print(data[1])
        if len(data) >0:
            comnumbuer = data[0]
        else:
            comnumbuer = '1'

        if len(data) > 1:
            binpath = data[1]
        else:
            binpath = ''

        textPortNum.AppendText(comnumbuer)
        filename.AppendText(binpath)
        f1.close()

    except Exception as e:
        print("----异常：", e)
    except FileNotFoundError:
        print('无法打开指定的文件!')
    except LookupError:
        print('指定了未知的编码!')
    except UnicodeDecodeError:
        print('读取文件时解码错误!')
    finally:
        if f1:
            f1.close()

def loadbutton(event):
    global filedata
    global filename
    assert isinstance(filename, wx.TextCtrl)

    dialog = wx.FileDialog(None,"打开文件", os.getcwd(),"","*.*|*.*", wx.FD_OPEN)
    if dialog.ShowModal() == wx.ID_OK:
        filename.Clear()
        filename.AppendText(dialog.GetPath())
        openbinfile(filename.GetValue(),1)
    dialog.Destroy()

bdevicestatus = False
def ReadData():
    global rxstr
    global bserisopen
    global bupdate
    global bsendbin
    global STRGLO
    global filedata
    global devicestatus
    global bdevicestatus
    while True:
        if bserisopen and ser.in_waiting:
            count = 0
            while count < ser.in_waiting:
                #RxData.append(hex(ser.read(1)))
                #STRGLO = hex(ser.read(1))
                #STRGLO = hex(int.from_bytes(ser.read(1)),byteorder='big')
                STRGLO = ser.read(ser.in_waiting)
                #STRGLO = str(ser.read(1), encoding="gbk")
                print(STRGLO)
                rxstr = STRGLO.decode(encoding="utf-8", errors="ignore")
                protext.AppendText(rxstr)

        if wait1second == STRGLO:
            bupdate = False
            devicestatus.SetLabelText('设备已经连接')
            time.sleep(1.1)
            bsendbin = True
            STRGLO = ""
            print("wait 1s finish")

        if bdevicestatus:
            rxstr = protext.GetValue()
            if len(rxstr) > 13:
                rxstr = rxstr[-14:]

            if rxstr.find('run new code') != -1:
                devicestatus.SetLabelText('设备升级成功')
                devicestatus.SetBackgroundColour('green')
                bdevicestatus = False

            if rxstr.find('run old code') != -1:
                devicestatus.SetLabelText('设备升级失败')
                devicestatus.SetBackgroundColour('black')
                bdevicestatus = False

            if rxstr.find('user code failed') != -1:
                devicestatus.SetLabelText('设备升级失败')
                devicestatus.SetBackgroundColour('black')
                bdevicestatus = False

        if bsendbin:
            print("send bin file")
            devicestatus.SetLabelText('开始升级文件')
            bdevicestatus = True
            ser.write(filedata)
            bsendbin = False
            print("send bin file finish")


        #print("thead read com\r\n")
        if bserisopen and bupdate:
            ser.write("\r\nupdate\r\n".encode("gbk"))
            time.sleep(0.1)
            txt = devicestatus.GetLabelText()
            txtlen = len(txt)
            if txtlen >= len('等待连接设备..........'):
                devicestatus.SetLabelText('等待连接设备.')
                devicestatus.SetBackgroundColour('white')
            else:
                devicestatus.SetLabelText('等待连接设备..........'[0:txtlen+1])
                devicestatus.SetBackgroundColour('white')

            print("\r\nupdate\r\n")
        if bserisopen == False:
            print("thead end\r\n")
            break

def closeserial():
    global bserisopen
    global ser
    bserisopen = False
    if (ser!= None and ser.is_open):
        ser.close()
        ser = None

def openserial(portx = "com5",bps = 115200):
    global ser
    global ti
    global bserisopen
    try:
        #portx = "com" + textPortNum.GetValue()
        print(portx,bps)
        timex = 5
        ser = serial.Serial(portx, bps, timeout=timex)
        if (ser is not None and ser.is_open):
            bserisopen = True
            ti = threading.Thread(target=ReadData)
            ti.start()
    except Exception as e:
        print("----异常：", e)
        box = wx.MessageDialog(None, "串口打开错误", '错误', wx.OK)
        box.ShowModal()
        box.Destroy()


def openbutton(event):
    global ser
    if (ser == None):
        openserial("com" + textPortNum.GetValue())


def closebutton(event):
    global ser
    print("关闭串口：", ser)
    if ser != None:
        closeserial()

def updatebutton(event):
    global bupdate
    global ser
    global filedata
    if not ser:
        openserial("com" + textPortNum.GetValue())
        if (not ser or ser.is_open == False):
            ser = None
            box = wx.MessageDialog(None,"串口打开错误", '错误', wx.OK)
            box.ShowModal()
            box.Destroy()
            return

    if not filedata:
        openbinfile(filename.GetValue(), 1)
        if not filedata:
            return
    bupdate = True
    print("开始升级程序")

class UpFrame(wx.Frame):
    def __init__(self, parent):
        global textPortNum
        global protext
        global filename
        global devicestatus
        wx.Frame.__init__(self, parent, -1, "锦都按键板升级")
        self.SetBackgroundColour(wx.Colour(224, 224, 224))
        self.SetSize((540, 600))
        self.Center()
        self.SetWindowStyle(wx.DEFAULT_FRAME_STYLE)
        self.Bind(wx.EVT_CLOSE,self.OnClose)
        wx.StaticText(self, -1,"串口号",(5, 5))
        textPortNum = wx.TextCtrl(self, pos=(50, 5), size=(80, 30))

        open_button = wx.Button(self, label="打开串口", pos=(140, 5), size=(80, 30))
        open_button.Bind(wx.EVT_BUTTON, openbutton)

        close_button = wx.Button(self, label="关闭串口", pos=(230, 5), size=(80, 30))
        close_button.Bind(wx.EVT_BUTTON, closebutton)

        filename = wx.TextCtrl(self, pos=(5, 40), size=(500, 30))

        update_button = wx.Button(self, label="打开文件", pos=(5, 75), size=(100, 40))
        update_button.Bind(wx.EVT_BUTTON, loadbutton)

        update_button = wx.Button(self, label="升级程序", pos=(110, 75), size=(100, 40))
        update_button.Bind(wx.EVT_BUTTON, updatebutton)

        devicestatus  = wx.StaticText(self, wx.ID_ANY,'', pos=(220,75), size=(160,40),style=wx.ALIGN_LEFT)
        devicestatus.SetForegroundColour('red')
        devicestatus.SetBackgroundColour('white')
        #devicestatus.SetLabelText('123')


        protext = wx.TextCtrl(self, pos=(5, 140), size=(500, 400), style=wx.TE_MULTILINE)

        opensettingfile()
    def OnClose(self, evt):
        dlg = wx.MessageDialog(self, u'确定要关闭本窗口？', u'操作提示', wx.YES_NO | wx.ICON_QUESTION)
        if (dlg.ShowModal() == wx.ID_YES):
            savesettingfile()
            closeserial()
            self.Destroy()

