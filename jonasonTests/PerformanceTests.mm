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
#include <vector>

@interface PerformanceTests : XCTestCase

@end

@implementation PerformanceTests


- (void) testPeformanceBenchmark {

    NSString* file_path = [[NSBundle bundleForClass:[self class]] pathForResource:@"canada" ofType:@"json"];

    [self measureMetrics:[self class].defaultPerformanceMetrics automaticallyStartMeasuring:NO forBlock:^{
        NSInputStream* istream = [[NSInputStream alloc] initWithFileAtPath: file_path];
        [istream open];
        NSError* error = nil;

        [self startMeasuring];
        [NSJSONSerialization JSONObjectWithStream:istream options:0 error: &error];
        [self stopMeasuring];

        if (error) { XCTFail(); }

        [istream close];
    }];
}

- (void) testParsePerformance {
    const char* file_path = [[[NSBundle bundleForClass:[self class]] pathForResource:@"canada" ofType:@"json"] UTF8String];

    [self measureMetrics:[self class].defaultPerformanceMetrics automaticallyStartMeasuring:NO forBlock:^{
        std::ifstream ifstream;
        ifstream.open(file_path, std::ifstream::in);

        [self startMeasuring];
        try {
            auto json = jonason::parse(ifstream);
        } catch (...) {
            XCTFail();
        }
        [self stopMeasuring];
    }];
}

- (void) testTokenizePerformance {
    const char* file_path = [[[NSBundle bundleForClass:[self class]] pathForResource:@"canada" ofType:@"json"] UTF8String];

    [self measureMetrics:[self class].defaultPerformanceMetrics automaticallyStartMeasuring:NO forBlock:^{
        std::ifstream ifstream;
        ifstream.open(file_path, std::ifstream::in);

        [self startMeasuring];
        std::vector<jonason::Token> tokens;
        jonason::tokenize(ifstream, tokens);
        [self stopMeasuring];
    }];
}

@end
