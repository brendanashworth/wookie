var http = require('http'),
	exec = require('child_process').exec,
	assert = require('chai').assert;

describe('wookie http server', function() {
	describe('wookie http parser', function() {

	});

	// gotta start the web server
	before(function(done) {
		this.timeout(10000);
		
		exec('./wookie 127.0.0.1 8080', function(error, stdout, stdin) {
			console.log(stdout);
			assert.notOk(error, 'gives error while booting');
			done();
		});
	});

	// make sure it make's fine!
	it('should compile without error', function(done) {
		exec('make', function(error, stdout, stdin) {
			assert.notOk(error, 'gives error on make');
			done();
		});
	});

	// does the server actually work?
	it('should give 200 on GET / request', function(done) {
		http.get('http://localhost:8080/', function(res) {
			assert.equal(200, res.statusCode, 'does not give 200 response code');
			done();
		});
	});
});