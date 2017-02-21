#! /usr/bin/env python
#
import os

from ROOT import TH1F, TFile


def writeListToFile(chfile,list,nbr_bad_ch):
    for ich in xrange(nbr_bad_ch):
        print 'ieta = {}, iphi = {}, value = {}'.format(list[ich][1],list[ich][2],list[ich][0])
        chfile.write( "    [{},{}],".format(list[ich][1],list[ich][2]) )
        chfile.write( "\n" )

file = TFile.Open("data/trackanddiffractive_sigDD_data.root")

hist = file.Get("data_ZeroBias1_CASTOR/Calotower2D_eta_phi")

# hist.Draw("colz")

# list with all channels
chlist = []

#list with channels |ieta|<=14
ch14list = []
#list with channels 14<|ieta|<=20
ch20list = []
#list with channels 20<|ieta|<=33
ch33list = []
#list with channels 33<|ieta|<inf
chRestlist = []

# loop over all bins OR SAY over the whole x-y range
for iXbin in xrange(hist.GetXaxis().GetNbins()):
    for iYbin in xrange(hist.GetYaxis().GetNbins()):
        value = hist.GetBinContent(iXbin+1,iYbin+1)
        ieta = int(hist.GetXaxis().GetBinCenter(iXbin+1))
        iphi = int(hist.GetYaxis().GetBinCenter(iYbin+1))

        chlist.append([value,ieta,iphi])

        if abs(ieta) <= 14:
            ch14list.append([value,ieta,iphi])
        elif abs(ieta) <= 20:
            ch20list.append([value,ieta,iphi])
        elif abs(ieta) <= 33:
            ch33list.append([value,ieta,iphi])
        else:
            chRestlist.append([value,ieta,iphi])

        # if ieta == -28 and iphi == 53:
        #     print '{}, {}, {}'.format(ieta,iphi,value)

chlist.sort(reverse=True)
ch14list.sort(reverse=True)
ch20list.sort(reverse=True)
ch33list.sort(reverse=True)
chRestlist.sort(reverse=True)

# for ich in chlist:
#     print 'ieta = {}, iphi = {}, value = {}'.format(ich[1],ich[2],ich[0])


chlist_file = open('bad_calo_channel_list.py', 'w')
chlist_file.write("bad_channels_eta_phi = [\n")

# is not used anymore
# number_of_bad_channels = 50
# for ich in xrange(number_of_bad_channels):
#     print 'ieta = {}, iphi = {}, value = {}'.format(chlist[ich][1],chlist[ich][2],chlist[ich][0])

#     chlist_file.write( "    [{},{}]".format(chlist[ich][1],chlist[ich][2]) )
#     if ich < number_of_bad_channels-1:
#         chlist_file.write(",\n")

number_bad_channels_14list = 10
number_bad_channels_20list = 50
number_bad_channels_33list = 50
number_bad_channels_Restlist = 10

writeListToFile(chlist_file,ch14list,number_bad_channels_14list)
writeListToFile(chlist_file,ch20list,number_bad_channels_20list)
writeListToFile(chlist_file,ch33list,number_bad_channels_33list)
writeListToFile(chlist_file,chRestlist,number_bad_channels_Restlist)

chlist_file.write("]\n")
chlist_file.close()