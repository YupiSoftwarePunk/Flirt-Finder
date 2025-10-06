namespace Server.Models
{
    public class MessageReport
    {
        public int Id { get; set; }
        public string ReporterId { get; set; }
        public int MessageId { get; set; }
        public string Reason { get; set; }
        public DateTime ReportedAt { get; set; }
    }
}
