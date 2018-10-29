#!/usr/bin/python
import urllib2
import sys
import binascii
import string

TARGET = 'http://crypto-class.appspot.com/po?er='
#--------------------------------------------------------------
# padding oracle
#--------------------------------------------------------------
class PaddingOracle(object):
    def query(self, q):
        target = TARGET + urllib2.quote(q)    # Create query URL
        print "target is : ", target
        req = urllib2.Request(target)         # Send HTTP request to server
        try:
            f = urllib2.urlopen(req)          # Wait for response
        except urllib2.HTTPError, e:          
            print "We got: %d" % e.code       # Print response code
            if e.code == 404:
                return 1 # good padding
            return 2 # bad padding
        print "We got: accepted"
        return 3


if __name__ == "__main__":
    po = PaddingOracle()
    ct_hex   = 'f20bdba6ff29eed7b046d1df9fb7000058b1ffb4210a580f748b4ac714c001bd4a61044426fb515dad3f21f18aa577c0bdf302936266926ff37dbf7035d5eeb4'
    ct_unhex = bytearray(binascii.unhexlify(ct_hex))
    print "ct_unhex len ", len(ct_unhex) , " is : ", ct_unhex

    m = bytearray(len(ct_unhex))

    # this means the length of python string is not related to zero end, unlike c and c++
    for i in range(len(ct_unhex)-1,15,-1):
      # this can be handled by changing last block's byte
      i_last=i-16
      pad = 16-(i%16)
      got = False
      for guest in range(0,256,1):
        x = ct_unhex[i_last] ^ guest ^ pad
        print "loop guest ",i,i_last,ct_unhex[i],guest,pad,x
        # constructing the fake ct
        # first copying al chars before the guesting position
        fake_ct_unhex = bytearray((i/16)*16+16)
        fake_ct_unhex[:i_last] = ct_unhex[:i_last]
        #faking with guest padding
        fake_ct_unhex[i_last] = x
        # faking with correct guest padding
        for fakepos in range(i_last+1,(i/16)*16,1):
          print "fakepos ", fakepos , " m ", m[fakepos +16]
          fake_ct_unhex[fakepos] = ct_unhex[fakepos] ^ m[fakepos+16] ^ pad 
        # continue copying the next block only
        fake_ct_unhex[(i/16)*16:(i/16)*16+16] = ct_unhex[(i/16)*16:(i/16)*16+16]
        # convert back to hexstring
        fake_ct_hex = binascii.hexlify(str(fake_ct_unhex))
        # try it
        if ct_unhex == fake_ct_hex:
          print "same cipher"
        res= po.query(fake_ct_hex)       # Issue HTTP query with the given argument
        if res == 1:
          print "good padding at ",i," value ", guest
          m[i]=guest
          got = True
          break
        elif (res==3) and (pad ==1):
          continue
        elif (res==3):
          print "good padding at ",i," value ", guest
          m[i]=guest
          got = True
          break

      if got == False:
        print "fail to find a guest"
        sys.exit(0)
