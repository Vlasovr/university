using MongoDB.Bson;
using MongoDB.Bson.Serialization.Attributes;

namespace CinemaWebService.Models
{
    public class Client
    {
        [BsonId] // Указывает, что это поле является идентификатором документа
        [BsonRepresentation(BsonType.ObjectId)] // Позволяет использовать строковое представление ObjectId
        public string? Id { get; set; } // Свойство для хранения _id из MongoDB опционально для него, для posgres оно должно неопциональным
        public int ClientId { get; set; } // PK
        public string Email { get; set; }
        public string FullName { get; set; }
        public string OrderHistory { get; set; }
        
        [BsonIgnore]
        public ICollection<Review> Reviews { get; set; } = new HashSet<Review>(); 
        [BsonIgnore]
        public ICollection<Ticket> Tickets { get; set; } = new HashSet<Ticket>(); 
    }
}