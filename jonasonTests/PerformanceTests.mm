//
//  PerformanceTests.m
//  jonasonTests
//
//  Created by Gabriel Lumbi on 2023-03-05.
//

#import <XCTest/XCTest.h>

#include <jonason/jonason.h>
#include <string>
#include <fstream>
#include <chrono>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

@interface PerformanceTests : XCTestCase

@end

@implementation PerformanceTests


- (void) testBenchmark {

    NSString* file_path = [[NSBundle bundleForClass:[self class]] pathForResource:@"canada" ofType:@"json"];

    [self measureMetrics:[self class].defaultPerformanceMetrics automaticallyStartMeasuring:NO forBlock:^{
        NSInputStream* istream = [[NSInputStream alloc] initWithFileAtPath: file_path];
        [istream open];
        NSError* error = nil;

        [self startMeasuring];
        [NSJSONSerialization JSONObjectWithStream:istream options:0 error: nil];
        [self stopMeasuring];

        if (error) { XCTFail(); }

        [istream close];
    }];
}

- (void) testPerformance {
    const char* file_path = [[[NSBundle bundleForClass:[self class]] pathForResource:@"canada" ofType:@"json"] UTF8String];

    [self measureMetrics:[self class].defaultPerformanceMetrics automaticallyStartMeasuring:NO forBlock:^{
        std::ifstream ifstream;
        ifstream.open(file_path, std::ifstream::in);

        [self startMeasuring];
        jonason::JSONValue* json = nullptr;
        try {
            jonason::parse(ifstream, json);
        } catch (...) {
            XCTFail();
        }
        [self stopMeasuring];
    }];
}


@end
