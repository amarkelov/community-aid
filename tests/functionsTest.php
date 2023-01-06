<?php
declare(strict_types=1);
require_once(dirname(dirname(__FILE__)) . '/webserver/php/functions.inc');
use PHPUnit\Framework\TestCase;

final class functionsTest extends TestCase
{
    public function test_verify_timeslot(): void
    {
        $timeslots = [ "09:00", "25:66" ];
        $this->assertSame("09:00", verify_timeslot($timeslots[0]));
        $this->assertSame("00:00", verify_timeslot($timeslots[1]));
    }

    public function test_filter_phone_number(): void
    {
        $phone_numbers = [ '322-223', '1800-bad-one' ];
        $this->assertSame('322223', filter_phone_number($phone_numbers[0]));
        $this->assertSame('N/A', filter_phone_number($phone_numbers[1]));
    }

    public function test_array2string(): void
    {
        $array = [ 123, 'test item', 'another one' ];
        $this->assertSame('123,test item,another one', array2string($array));

        $string = '123, and one more';
        $this->assertSame('', array2string($string));
    }
}
?>
