var assert = require('assert');
var openthread = require('openthread');

const kDevicePath = '/dev/ttyUSB0';

describe('OpenThread', function() {
  describe('#open', function() {
    it('should open radio transceiver', function() {
        let ot = openthread.open(kDevicePath, '115200');
        assert(ot.version);
    });

    it('should be able to form network', function(done) {
        let ot = openthread.open(kDevicePath, '115200');
        ot.start({
            network_name: 'OpenThread',
            password: '12345678',
        }, function(){
            assert(ot.state != 'disabled');
            done();
        });
    });
  });
});
