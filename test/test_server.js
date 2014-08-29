var http = require('http'),
	exec = require('child_process').exec,
	assert = require('chai').assert;

describe('wookie http server', function() {
	describe('wookie http parser', function() {

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