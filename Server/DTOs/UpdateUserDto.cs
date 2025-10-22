namespace Server.DTOs
{
    public class UpdateUserDto
    {
        public string FullName { get; set; }
        public string Bio { get; set; }
        public string Gender { get; set; }
        public int Age { get; set; }
        public string PhotoUrl { get; set; }
        public string City { get; internal set; }
    }
}
