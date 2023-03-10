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

// canada.json

- (void) testPeformanceBenchmark1 {
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

- (void) testParsePerformance1 {
    const char* file_path = [[[NSBundle bundleForClass:[self class]] pathForResource:@"canada" ofType:@"json"] UTF8String];

    [self measureMetrics:[self class].defaultPerformanceMetrics automaticallyStartMeasuring:NO forBlock:^{
        std::ifstream ifstream;
        ifstream.open(file_path, std::ifstream::in);

        [self startMeasuring];
        try {
            jonason::parse(ifstream);
        } catch (...) {
            XCTFail();
        }
        [self stopMeasuring];
    }];
}

- (void) testTokenizePerformance1 {
    const char* file_path = [[[NSBundle bundleForClass:[self class]] pathForResource:@"canada" ofType:@"json"] UTF8String];

    [self measureMetrics:[self class].defaultPerformanceMetrics automaticallyStartMeasuring:NO forBlock:^{
        std::ifstream ifstream;
        ifstream.open(file_path, std::ifstream::in);

        jonason::Tokenizer tokenizer(ifstream);
        [self startMeasuring];
        while (true) {
            jonason::Token token = tokenizer.get_next();
            if (token.is_eof()) break;
        }
        [self stopMeasuring];
    }];
}

// citm_catalog.json

- (void) testPeformanceBenchmark2 {
    NSString* file_path = [[NSBundle bundleForClass:[self class]] pathForResource:@"citm_catalog" ofType:@"json"];

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

- (void) testParsePerformance2 {
    const char* file_path = [[[NSBundle bundleForClass:[self class]] pathForResource:@"citm_catalog" ofType:@"json"] UTF8String];

    [self measureMetrics:[self class].defaultPerformanceMetrics automaticallyStartMeasuring:NO forBlock:^{
        std::ifstream ifstream;
        ifstream.open(file_path, std::ifstream::in);

        [self startMeasuring];
        try {
            jonason::parse(ifstream);
        } catch (...) {
            XCTFail();
        }
        [self stopMeasuring];
    }];
}

- (void) testTokenizePerformance2 {
    const char* file_path = [[[NSBundle bundleForClass:[self class]] pathForResource:@"citm_catalog" ofType:@"json"] UTF8String];

    [self measureMetrics:[self class].defaultPerformanceMetrics automaticallyStartMeasuring:NO forBlock:^{
        std::ifstream ifstream;
        ifstream.open(file_path, std::ifstream::in);

        jonason::Tokenizer tokenizer(ifstream);
        [self startMeasuring];
        while (true) {
            jonason::Token token = tokenizer.get_next();
            if (token.is_eof()) break;
        }
        [self stopMeasuring];
    }];
}

@end
